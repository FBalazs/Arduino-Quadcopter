package fb.quad.control;

import javax.swing.JOptionPane;

public class Error {
	public static void popUp(Exception e) {
		e.printStackTrace();
		new JOptionPane("Error: "+e.toString(), JOptionPane.ERROR_MESSAGE).createDialog(null, "Error").setVisible(true);
	}
}
