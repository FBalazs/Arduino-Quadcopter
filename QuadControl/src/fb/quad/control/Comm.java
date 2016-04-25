package fb.quad.control;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

public class Comm implements Runnable {
	public static interface CommListener {
		void onCharReceived(char c);
		void onTPSCommand(int tps);
		void onMPUCommand(int sps);
		void onPosCommand(float p, float y, float r);
		void onUnknownCommand(String cmd, String[] params);
	}
	
	private boolean closed = false;
	
	private SerialPort serialPort;
	private InputStream in;
	private OutputStream out;
	
	private CommListener listener;
	
	public Comm(String commPort, int baud, CommListener listener) {
		try {
			serialPort = (SerialPort)CommPortIdentifier.getPortIdentifier(commPort).open("QuadControl", 1000);
			serialPort.setSerialPortParams(baud, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
			in = serialPort.getInputStream();
			out = serialPort.getOutputStream();
		} catch (Exception e) {
			Error.popUp(e);
		}
		
		this.listener = listener;
	}
	
	public synchronized int available() {
		try {
			return in.available();
		} catch (IOException e) {
			e.printStackTrace();
			return -1;
		}
	}
	
	public synchronized void writeString(String str) {
		try {
			out.write(str.getBytes());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public synchronized char readChar() {
		try {
			return (char)in.read();
		} catch (IOException e) {
			e.printStackTrace();
			return '\0';
		}
	}
	
	private synchronized void parseCommand(String cmd, String[] params) {
		try {
			if(cmd.equals("tps") && params.length == 1) {
				listener.onTPSCommand(Integer.parseInt(params[0]));
				return;
			}
			if(cmd.equals("mpuSamples") && params.length == 1) {
				listener.onMPUCommand(Integer.parseInt(params[0]));
				return;
			}
			if(cmd.equals("pos") && params.length == 3) {
				listener.onPosCommand(Float.parseFloat(params[0]),
						Float.parseFloat(params[1]),
						Float.parseFloat(params[2]));
				return;
			}
		} catch(Exception e) {
			e.printStackTrace();
		}
		listener.onUnknownCommand(cmd, params);
	}
	
	StringBuilder ccmd = new StringBuilder();
	
	public synchronized void update() {
		if(0 < available()) {
			char c = readChar();
			listener.onCharReceived(c);
			if(c == '\n') {
				StringBuilder cmd = new StringBuilder();
				int pn = 0;
				for(int i = 0; i < ccmd.length(); ++i) {
					if(ccmd.charAt(i) == '\t' || ccmd.charAt(i) == ' ')
						++pn;
					if(pn == 0)
						cmd.append(ccmd.charAt(i));
				}
				String[] params = new String[pn];
				int i = 0;
				StringBuilder cparam = new StringBuilder();
				for(int j = cmd.length()+1; j < ccmd.length(); ++j)
					if(ccmd.charAt(j) == '\t' || ccmd.charAt(j) == ' ') {
						params[i++] = cparam.toString();
						cparam = new StringBuilder();
					} else
						cparam.append(ccmd.charAt(j));
				if(pn != 0)
					params[i] = cparam.toString();
				parseCommand(cmd.toString(), params);
				ccmd = new StringBuilder();
			} else
				ccmd.append(c);
		}
	}
	
	public synchronized void close() {
		closed = true;
		if(serialPort != null)
			serialPort.close();
	}
	
	@Override
	public void run() {
		while(!closed) {
			update();
		}
	}
}
