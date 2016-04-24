package fb.quad.control;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.Enumeration;

import javax.swing.JFrame;

import gnu.io.CommPortIdentifier;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import java.awt.GridBagLayout;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.JComboBox;

public class ConnectWindow extends JFrame implements WindowListener {
	private static final long serialVersionUID = -9107016169908333507L;
	
	private JComboBox<String> cbPort;
	private JComboBox<Integer> cbBaud;
	private JButton btnRefresh;
	private JButton btnConnect;
	
	private void refreshSpinner() {
		DefaultComboBoxModel<String> cbm = (DefaultComboBoxModel)cbPort.getModel();
		cbm.removeAllElements();
		Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
		while(portEnum.hasMoreElements()) {
			CommPortIdentifier portIdentifier = portEnum.nextElement();
			cbm.addElement(portIdentifier.getName());
		}
	}
	
	public ConnectWindow() {
		super("QuadControl");
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[]{20, 0, 0, 20};
		gridBagLayout.rowHeights = new int[]{20, 0, 0, 0, 20};
		gridBagLayout.columnWeights = new double[]{0.0, 1.0, 1.0, 0.0};
		gridBagLayout.rowWeights = new double[]{0.0, Double.MIN_VALUE, Double.MIN_VALUE, Double.MIN_VALUE, 0.0};
		getContentPane().setLayout(gridBagLayout);
		
		cbPort = new JComboBox<String>();
		GridBagConstraints gbc_cbPort = new GridBagConstraints();
		gbc_cbPort.anchor = GridBagConstraints.EAST;
		gbc_cbPort.insets = new Insets(0, 0, 5, 5);
		gbc_cbPort.gridx = 1;
		gbc_cbPort.gridy = 1;
		getContentPane().add(cbPort, gbc_cbPort);
		
		cbBaud = new JComboBox<Integer>();
		cbBaud.setModel(new DefaultComboBoxModel<>(new Integer[]{9600, 19200, 38400, 57600, 74880, 115200, 230400, 250000}));
		GridBagConstraints gbc_cbBaud = new GridBagConstraints();
		gbc_cbBaud.anchor = GridBagConstraints.WEST;
		gbc_cbBaud.insets = new Insets(0, 0, 5, 5);
		gbc_cbBaud.gridx = 2;
		gbc_cbBaud.gridy = 1;
		getContentPane().add(cbBaud, gbc_cbBaud);
		
		btnRefresh = new JButton("Refresh");
		GridBagConstraints gbc_btnRefresh = new GridBagConstraints();
		gbc_btnRefresh.gridwidth = 2;
		gbc_btnRefresh.insets = new Insets(0, 0, 5, 5);
		gbc_btnRefresh.gridx = 1;
		gbc_btnRefresh.gridy = 2;
		getContentPane().add(btnRefresh, gbc_btnRefresh);
		
		btnConnect = new JButton("Connect");
		GridBagConstraints gbc_btnConnect = new GridBagConstraints();
		gbc_btnConnect.insets = new Insets(0, 0, 0, 5);
		gbc_btnConnect.gridwidth = 2;
		gbc_btnConnect.gridx = 1;
		gbc_btnConnect.gridy = 3;
		getContentPane().add(btnConnect, gbc_btnConnect);
		
		btnRefresh.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				refreshSpinner();
			}
		});
		
		btnConnect.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				new ControlWindow((String)cbPort.getSelectedItem(), (Integer)cbBaud.getSelectedItem()).setVisible(true);
				setVisible(false);
			}
		});
		
		refreshSpinner();
		
		setMinimumSize(new Dimension(200, 150));
		setSize(new Dimension(250, 150));
		setLocationRelativeTo(null);
	}
	
	@Override
	public void windowOpened(WindowEvent e) {}
	
	@Override
	public void windowClosing(WindowEvent e) {
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
