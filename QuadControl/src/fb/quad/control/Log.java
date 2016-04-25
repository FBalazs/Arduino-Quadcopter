package fb.quad.control;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Log extends PrintWriter {

	private static String superAsd(String dir) {
		new File(dir).mkdirs();
		return dir+"/"+new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(new Date())+".log";
	}
	
	public Log(String dir) throws FileNotFoundException {
		super(superAsd(dir));
	}
}
