/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bitmapencoder;

import java.io.IOException;
import javax.imageio.ImageIO;
import java.io.File;
import java.awt.image.*;

/**
 *
 * @author Rodot
 */
public class BitmapEncoder {

    private BufferedImage inputImage;
    private BufferedImage outputImage;
    private String bitmapName = "myBitmap";
    private boolean hexFormatting = false;
    private boolean wrapping = true;
    private static File[] filesToConvert;
    private static int imageCounter;
    private static int fileCount;

    protected void BitmapEncoder() {
    }

    protected void open(File file) {
        try {
            inputImage = ImageIO.read(file);
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (inputImage != null) {
            if ((inputImage.getWidth() > 200) || (inputImage.getHeight() > 200)) {
                inputImage = null;
                return;
            }
            outputImage = deepCopy(inputImage);
        }
    }

    protected void threshold(int thres) {
        if (inputImage == null) {
            return;
        }
        for (int y = 0; y < inputImage.getHeight(); y++) {
            for (int x = 0; x < inputImage.getWidth(); x++) {
                int rgb = inputImage.getRGB(x, y);
                int red = (rgb >> 16) & 0x000000FF;
                int green = (rgb >> 8) & 0x000000FF;
                int blue = (rgb) & 0x000000FF;
                int value = red + green + blue;
                if (value > thres) {
                    outputImage.setRGB(x, y, 0x00FFFFFF);
                } else {
                    outputImage.setRGB(x, y, 0);
                }
            }
        }
    }

    protected String generateOutput(int thres) {
        if (inputImage == null) {
            return "";
        }
        String output = "";
        output = output.concat("const byte ");
        output = output.concat(bitmapName);
        output = output.concat("[] PROGMEM = {");
        int width = ((inputImage.getWidth() - 1) / 8 + 1) * 8; //round to the closest larger multiple of 8
        output = output.concat(width + "," + inputImage.getHeight() + ",");
        if (wrapping) {
            output = output.concat("\n");
        }
        for (int y = 0; y < inputImage.getHeight(); y++) {
            for (int x = 0; x < inputImage.getWidth(); x += 8) {
                if (hexFormatting) {
                    output = output.concat("0x");
                } else {
                    output = output.concat("B");
                }
                int thisByte = 0;
                for (int b = 0; b < 8; b++) {
                    int value = 0xFFFF;
                    if (x + b < inputImage.getWidth()) {
                        int rgb = inputImage.getRGB(x + b, y);
                        int red = (rgb >> 16) & 0x000000FF;
                        int green = (rgb >> 8) & 0x000000FF;
                        int blue = (rgb) & 0x000000FF;
                        value = red + green + blue;
                    }
                    if (hexFormatting) {
                        thisByte *= 2;
                        if (value < thres) {
                            thisByte++;
                        }
                    } else {//binary formattning
                        if (value < thres) {
                            output = output.concat("1");
                        } else {
                            output = output.concat("0");
                        }

                    }
                }
                if (hexFormatting) {
                    output = output.concat(Integer.toString(thisByte, 16).
                            toUpperCase());
                }
                output = output.concat(",");
            }
            if (wrapping) {
                output = output.concat("\n");
            }
        }
        output = output.concat("};");
        return output;
    }

    protected static BufferedImage deepCopy(BufferedImage bi) {
        ColorModel cm = bi.getColorModel();
        boolean isAlphaPremultiplied = cm.isAlphaPremultiplied();
        WritableRaster raster = bi.copyData(null);
        return new BufferedImage(cm, raster, isAlphaPremultiplied, null);
    }

    protected static int countFiles(File[] list, boolean recursed) {

        if (list == null) {
            return 0;
        }

        if (!recursed) {
            fileCount = 0;
        }

        for (File f : list) {
            if (f.isDirectory()) {
                countFiles(f.listFiles(), true);
            } else {
                if (isImage(f)) {
                    fileCount++;
                }
            }
        }
        return fileCount;
    }

    protected static boolean isImage(File f) {
        boolean isImage = false;
        if (f.getName().endsWith(".bmp") || f.getName().endsWith(".png")
                || f.getName().endsWith(".jpeg")
                || f.getName().endsWith(".jpg")) {
            isImage = true;
        }
        return isImage;
    }

    protected static File[] processSelectedFiles(File[] list, boolean recursed) {
        if (!recursed) {
            filesToConvert = new File[countFiles(list, false)];
            imageCounter = 0;
        }
        if (list == null) {
            return null;
        }

        for (File f : list) {
            if (f.isDirectory()) {
                processSelectedFiles(f.listFiles(), true);
            } else {
                if (isImage(f)) {
                    filesToConvert[imageCounter] = f;
                    imageCounter++;
                }
            }
        }
        return filesToConvert;
    }

    protected BufferedImage getInputImage() {
        return inputImage;
    }

    protected BufferedImage getOutputImage() {
        return outputImage;
    }

    protected String getBitmapName() {
        return bitmapName;
    }

    protected boolean isHexFormatting() {
        return hexFormatting;
    }

    protected boolean isWrapping() {
        return wrapping;
    }

    protected void setInputImage(BufferedImage inputImage) {
        this.inputImage = inputImage;
    }

    protected void setOutputImage(BufferedImage outputImage) {
        this.outputImage = outputImage;
    }

    protected void setBitmapName(String bitmapName) {
        this.bitmapName = bitmapName;
    }

    protected void setHexFormatting(boolean hexFormatting) {
        this.hexFormatting = hexFormatting;
    }

    protected void setWrapping(boolean wrapping) {
        this.wrapping = wrapping;
    }
}
