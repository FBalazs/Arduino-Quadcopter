package fb.quad.control;

import java.awt.Dimension;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.swing.JFrame;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JCheckBoxMenuItem;
import java.awt.GridBagLayout;
import javax.swing.JTextArea;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.SwingConstants;
import java.awt.GridLayout;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

public class ControlWindow extends JFrame implements WindowListener {
	private static final long serialVersionUID = -8274994178068566823L;

	private SerialPort serialPort;
	private InputStream in;
	private OutputStream out;
	
	private boolean closing = false;
	
	private JMenu mnViews;
		private JCheckBoxMenuItem cbMiConsole;
	private JMenu mnViewOptions;
		private JMenu mnConsoleView;
			private JCheckBoxMenuItem cbMiConsoleViewAutoscroll;
	
	private JPanel panelControl;
	private JSlider slidLift;
	private JSlider slidPitch;
	private JSlider slidYaw;
	private JSlider slidRoll;
	
	private JScrollPane panelConsole;
	private JTextArea taConsole;
	
	public ControlWindow(String commPort, int baud) {
		super("QuadControl");
		setResizable(true);
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		
		try {
			serialPort = (SerialPort)CommPortIdentifier.getPortIdentifier(commPort).open("QuadControl", 1000);
			serialPort.setSerialPortParams(baud, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
			in = serialPort.getInputStream();
			out = serialPort.getOutputStream();
		} catch (Exception e) {
			Error.popUp(e);
		}
		
		setMinimumSize(new Dimension(400, 300));
		setSize(new Dimension(800, 600));
		setLocationRelativeTo(null);
		
		JMenuBar menuBar = new JMenuBar();
		setJMenuBar(menuBar);
		
		mnViews = new JMenu("Views");
		menuBar.add(mnViews);
		
		cbMiConsole = new JCheckBoxMenuItem("Console");
		cbMiConsole.setSelected(true);
		mnViews.add(cbMiConsole);
		
		mnViewOptions = new JMenu("View Options");
		menuBar.add(mnViewOptions);
		
		mnConsoleView = new JMenu("Console View");
		mnViewOptions.add(mnConsoleView);
		
		cbMiConsoleViewAutoscroll = new JCheckBoxMenuItem("Autoscroll");
		cbMiConsoleViewAutoscroll.setSelected(true);
		mnConsoleView.add(cbMiConsoleViewAutoscroll);
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[]{20, 0, 20, 0, 20};
		gridBagLayout.rowHeights = new int[]{20, 0, 20, 0, 20};
		gridBagLayout.columnWeights = new double[]{0.0, 1.0, 0.0, 1.0, 0.0};
		gridBagLayout.rowWeights = new double[]{0.0, 1.0, 0.0, 1.0, 0.0};
		getContentPane().setLayout(gridBagLayout);
		
		panelControl = new JPanel();
		GridBagConstraints gbc_panelControl = new GridBagConstraints();
		gbc_panelControl.insets = new Insets(0, 0, 5, 5);
		gbc_panelControl.fill = GridBagConstraints.BOTH;
		gbc_panelControl.gridx = 1;
		gbc_panelControl.gridy = 1;
		getContentPane().add(panelControl, gbc_panelControl);
		panelControl.setLayout(new GridLayout(0, 4, 0, 0));
		
		slidLift = new JSlider();
		slidLift.setValue(0);
		slidLift.setToolTipText("");
		slidLift.setOrientation(SwingConstants.VERTICAL);
		panelControl.add(slidLift);
		
		slidPitch = new JSlider();
		slidPitch.setOrientation(SwingConstants.VERTICAL);
		panelControl.add(slidPitch);
		
		slidYaw = new JSlider();
		slidYaw.setOrientation(SwingConstants.VERTICAL);
		panelControl.add(slidYaw);
		
		slidRoll = new JSlider();
		slidRoll.setOrientation(SwingConstants.VERTICAL);
		panelControl.add(slidRoll);
		
		panelConsole = new JScrollPane();
		panelConsole.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		GridBagConstraints gbc_panelConsole = new GridBagConstraints();
		gbc_panelConsole.gridwidth = 3;
		gbc_panelConsole.insets = new Insets(0, 0, 5, 5);
		gbc_panelConsole.fill = GridBagConstraints.BOTH;
		gbc_panelConsole.gridx = 1;
		gbc_panelConsole.gridy = 3;
		getContentPane().add(panelConsole, gbc_panelConsole);
		
		taConsole = new JTextArea();
		panelConsole.setViewportView(taConsole);
		taConsole.setEditable(false);
		taConsole.setTabSize(4);
		
		new Thread() {
			@Override
			public void run() {
				try {
					while(!closing) {
						if(0 < in.available()) {
							readChar();
							if(cbMiConsoleViewAutoscroll.getState())
								panelConsole.getVerticalScrollBar().setValue(panelConsole.getVerticalScrollBar().getMaximum());
						}
					}
				} catch(Exception e) {
					Error.popUp(e);
				}
			}
		}.start();
		
		new Thread() {
			@Override
			public void run() {
				long timeStep = 1000/50;
				long timeCurrent;
				while(!closing) {
					timeCurrent = System.currentTimeMillis();
					writeString("ch "
								+(slidLift.getValue()/(float)slidLift.getMaximum())
								+" "+(slidPitch.getValue()/(float)slidPitch.getMaximum() - 0.5f)*30f
								+" "+(slidYaw.getValue()/(float)slidYaw.getMaximum() - 0.5f)*30f
								+" "+(slidRoll.getValue()/(float)slidRoll.getMaximum() - 0.5f)*30f
								+"\n", false);
					timeCurrent = System.currentTimeMillis()-timeCurrent;
					if(timeCurrent < timeStep) {
						try {
							Thread.sleep(timeStep-timeCurrent);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			}
		}.start();
	}
	
	private synchronized void writeString(String str) {
		writeString(str, true);
	}
	
	private synchronized void writeString(String str, boolean log) {
		try {
			out.write(str.getBytes());
			if(log)
				taConsole.setText(taConsole.getText()+str);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private synchronized char readChar() {
		return readChar(true);
	}
	
	private synchronized char readChar(boolean log) {
		try {
			char c = (char)in.read();
			if(log)
				taConsole.setText(taConsole.getText()+c);
			return c;
		} catch (IOException e) {
			e.printStackTrace();
			return '\0';
		}
	}
	
	@Override
	public void windowOpened(WindowEvent e) {}
	
	@Override
	public void windowClosing(WindowEvent e) {
		closing = true;
		if(serialPort != null)
			serialPort.close();
		System.exit(0);
	}

	@Override
	public void windowClosed(WindowEvent e) {}
	
	@Override
	public void windowActivated(WindowEvent e) {}

	@Override
	public void windowDeactivated(WindowEvent e) {}
	
	@Override
	public void windowIconified(WindowEvent e) {}

	@Override
	public void windowDeiconified(WindowEvent e) {}
}
