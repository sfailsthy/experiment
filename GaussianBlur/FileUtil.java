package sfailsthy;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class FileUtil {

	public static BufferedImage loadImg(String fileName) {
		String path = "C:\\Users\\Administrator\\Desktop\\";
		File imgFile = new File(path + fileName);
		try {
			return ImageIO.read(imgFile);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}
}
