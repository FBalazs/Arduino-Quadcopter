package fb.quad.control;

import javax.swing.UIManager;

public class Main {
	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {
			Error.popUp(e);
		}
		new ConnectWindow().setVisible(true);
	}
}
