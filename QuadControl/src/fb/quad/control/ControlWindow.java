package fb.quad.control;

import java.awt.Dimension;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JFrame;

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
import javax.swing.JLabel;

public class ControlWindow extends JFrame implements WindowListener, Comm.CommListener {
	private static final long serialVersionUID = -8274994178068566823L;
	
	private boolean closing = false;
	private Comm comm;
	
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
	
	private JPanel panelState;
		private JLabel lblTPS;
		private JLabel lblPitch;
		private JLabel lblYaw;
		private JLabel lblRoll;
	
	public ControlWindow(String commPort, int baud) {
		super("QuadControl");
		setResizable(true);
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		
		comm = new Comm(commPort, baud, this);
		
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
		gridBagLayout.columnWidths = new int[]{20, 300, 20, 300, 20};
		gridBagLayout.rowHeights = new int[]{20, 300, 20, 300, 20};
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
		
		panelState = new JPanel();
		GridBagConstraints gbc_panelState = new GridBagConstraints();
		gbc_panelState.insets = new Insets(0, 0, 5, 5);
		gbc_panelState.fill = GridBagConstraints.BOTH;
		gbc_panelState.gridx = 3;
		gbc_panelState.gridy = 1;
		getContentPane().add(panelState, gbc_panelState);
		GridBagLayout gbl_panelState = new GridBagLayout();
		gbl_panelState.columnWidths = new int[]{10, 0, 10};
		gbl_panelState.rowHeights = new int[]{10, 0, 0, 0, 0, 0};
		gbl_panelState.columnWeights = new double[]{0.0, 1.0, 0.0};
		gbl_panelState.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		panelState.setLayout(gbl_panelState);
		
		lblTPS = new JLabel("TPS:");
		GridBagConstraints gbc_lblTPS = new GridBagConstraints();
		gbc_lblTPS.anchor = GridBagConstraints.WEST;
		gbc_lblTPS.insets = new Insets(0, 0, 5, 0);
		gbc_lblTPS.gridx = 1;
		gbc_lblTPS.gridy = 1;
		panelState.add(lblTPS, gbc_lblTPS);
		
		lblPitch = new JLabel("pitch:");
		GridBagConstraints gbc_lblPitch = new GridBagConstraints();
		gbc_lblPitch.anchor = GridBagConstraints.WEST;
		gbc_lblPitch.insets = new Insets(0, 0, 5, 0);
		gbc_lblPitch.gridx = 1;
		gbc_lblPitch.gridy = 2;
		panelState.add(lblPitch, gbc_lblPitch);
		
		lblYaw = new JLabel("yaw:");
		GridBagConstraints gbc_lblYaw = new GridBagConstraints();
		gbc_lblYaw.anchor = GridBagConstraints.WEST;
		gbc_lblYaw.insets = new Insets(0, 0, 5, 0);
		gbc_lblYaw.gridx = 1;
		gbc_lblYaw.gridy = 3;
		panelState.add(lblYaw, gbc_lblYaw);
		
		lblRoll = new JLabel("roll:");
		GridBagConstraints gbc_lblRoll = new GridBagConstraints();
		gbc_lblRoll.anchor = GridBagConstraints.WEST;
		gbc_lblRoll.gridx = 1;
		gbc_lblRoll.gridy = 4;
		panelState.add(lblRoll, gbc_lblRoll);
		
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
		
		new Thread(comm).start();
		
		new Thread() {
			@Override
			public void run() {
				long timeStep = 1000/50;
				long timeCurrent;
				while(!closing) {
					timeCurrent = System.currentTimeMillis();
					comm.writeString("ch "
								+(slidLift.getValue()/(float)slidLift.getMaximum())
								+" "+(slidPitch.getValue()/(float)slidPitch.getMaximum() - 0.5f)*30f
								+" "+(slidYaw.getValue()/(float)slidYaw.getMaximum() - 0.5f)*30f
								+" "+(slidRoll.getValue()/(float)slidRoll.getMaximum() - 0.5f)*30f
								+"\n");
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
	
	@Override
	public void onCharReceived(char c) {
		/*taConsole.setText(taConsole.getText()+c);
		if(cbMiConsoleViewAutoscroll.getState());
			panelConsole.getVerticalScrollBar().setValue(panelConsole.getVerticalScrollBar().getMaximum());*/
	}

	@Override
	public void onTPSCommand(int tps) {
		lblTPS.setText("TPS: "+tps);
	}

	@Override
	public void onMPUCommand(int sps) {
		
	}

	@Override
	public void onPosCommand(float p, float y, float r) {
		lblPitch.setText("pitch: "+p);
		lblYaw.setText("yaw: "+y);
		lblRoll.setText("roll: "+r);
	}

	@Override
	public void onUnknownCommand(String cmd, String[] params) {
		System.out.println("---------------");
		System.out.println("Unknown command!");
		System.out.println("cmd=\""+cmd+"\"");
		System.out.println("params.length="+params.length);
		for(int i = 0; i < params.length; ++i)
			System.out.println("params["+i+"]=\""+params[i]+"\"");
		System.out.println("---------------");
	}
	
	@Override
	public void windowOpened(WindowEvent e) {}
	
	@Override
	public void windowClosing(WindowEvent e) {
		closing = true;
		comm.close();
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
