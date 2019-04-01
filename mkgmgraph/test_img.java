import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.awt.RenderingHints;
import java.awt.geom.Line2D.Double;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.net.Socket;
import java.text.DecimalFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.StringTokenizer;
import javax.imageio.ImageIO;

public class test_img
{
  public static void main(String[] paramArrayOfString)
  {
    int i = 2;
    int j = 3;
    int k = 320;
    int m = 240;
    int n = 410;
    int i1 = 240;
    int i2 = 0;
    int i3 = 100;
    
    DecimalFormat localDecimalFormat1 = new DecimalFormat("0000");
    DecimalFormat localDecimalFormat2 = new DecimalFormat("00");
    
    Date localDate = new Date();
    Calendar localCalendar = Calendar.getInstance();
    
    int i4 = localCalendar.get(1);
    int i5 = localCalendar.get(2) + 1;
    int i6 = localCalendar.get(5);
    
    int i7 = localCalendar.get(11);
    int i8 = localCalendar.get(12);
    
    String str1 = localDecimalFormat1.format(i4);
    String str2 = localDecimalFormat2.format(i5);
    String str3 = localDecimalFormat2.format(i6);
    String str4 = localDecimalFormat2.format(i7);
    String str5 = localDecimalFormat2.format(i8);
    
    BufferedImage localBufferedImage = new BufferedImage(n, i1, 4);
    try
    {
      localBufferedImage = ImageIO.read(new File("11062008.jpg"));
    }
    catch (Exception localException1)
    {
      localException1.printStackTrace();
    }
    Graphics2D localGraphics2D = localBufferedImage.createGraphics();
    localGraphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
    
    String str6 = "" + str1 + str2 + str3 + ".";
    
    String str8 = "wizg.org";
    int i9 = 80;
    
    int i10 = 30;int i11 = 30;int i12 = 40;int i13 = 3;
    int i14 = 0;
    int i15 = 0;
    int i16 = 0;int i17 = 0;int i18 = 0;
    int i19 = 0;int i20 = 0;int i21 = 0;
    int i22 = 0;
    
    String str7 = "http://" + str8 + "/demo/gmdata_" + str6 + "csv";
    
    System.out.println(str7);
    try
    {
      k = i * 24 * 6;
      localGraphics2D.setColor(Color.BLACK);
      localGraphics2D.drawLine(0 + i3, m - 1 + i2, k + i3, m - 1 + i2);
      
      localGraphics2D.drawLine(0 + i3, m - 10 * j + i2, k + i3, m - 10 * j + i2);
      for (i14 = 0; i14 < 8; i14++) {
        localGraphics2D.drawLine(0 + i3, 10 * i14 * j + i2, k + i3, 10 * i14 * j + i2);
      }
      localGraphics2D.drawLine(0 + i3, 0 + i2, 0 + i3, m + i2);
      for (i14 = 12; i14 < 144; i14 += 12) {
        localGraphics2D.drawLine(i * i14 + i3, j * 10 + i2, i * i14 + i3, m + i2);
      }
      localGraphics2D.drawLine(i * i14 + i3, 0 + i2, i * i14 + i3, m + i2);
      
      localGraphics2D.setColor(Color.BLUE);
      localGraphics2D.drawString("DATE: " + str1 + "/" + str2 + "/" + str3 + " (" + str4 + ":" + str5 + ")", 7 + i3, 15 + i2);
      
      Socket localSocket = new Socket(str8, i9);
      BufferedReader localBufferedReader = new BufferedReader(new InputStreamReader(localSocket.getInputStream()));
      
      BufferedWriter localBufferedWriter = new BufferedWriter(new OutputStreamWriter(localSocket.getOutputStream()));
      
      localBufferedWriter.write("GET " + str7 + " HTTP/1.1\r\n");
      localBufferedWriter.write("Host: " + str8 + "\r\n");
      localBufferedWriter.write("Connection: close\r\n");
      localBufferedWriter.write("\r\n");
      localBufferedWriter.flush();
      
      String str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      str9 = localBufferedReader.readLine();
      System.out.println(str9);
      
      int i23 = 0;
      
      i15 = 0;
      while ((str9 = localBufferedReader.readLine()) != null)
      {
        System.out.println(str9);
        
        i16 = i15 * 1;
        
        StringTokenizer localStringTokenizer = new StringTokenizer(str9, ",");
        localStringTokenizer.nextToken();
        localStringTokenizer.hasMoreTokens();
        i17 = Integer.valueOf(localStringTokenizer.nextToken().replaceAll(" ", "")).intValue() * j;
        
        i18 = Integer.valueOf(localStringTokenizer.nextToken().replaceAll(" ", "")).intValue() * j / 100;
        if (i18 > 700)
        {
          i18 /= 5;
          i22 = 1;
        }
        else
        {
          i22 = 0;
        }
        if (i15 == 0)
        {
          i19 = i16;
          i20 = i17;
          i21 = i18;
        }
        i23 += i17;
        
        localGraphics2D.setPaint(Color.red);
        localGraphics2D.draw(new Line2D.Double(i19 * i + i3, m - i20 + i2, i16 * i + i3, m - i17 + i2));
        if (i22 == 0) {
          localGraphics2D.setPaint(Color.green);
        } else {
          localGraphics2D.setPaint(Color.magenta);
        }
        localGraphics2D.draw(new Line2D.Double(i19 * i + i3, m - i21 + i2, i16 * i + i3, m - i18 + i2));
        
        i15++;
        i19 = i16;
        i20 = i17;
        i21 = i18;
      }
      localBufferedWriter.close();
      localBufferedReader.close();
      localSocket.close();
      
      int i24 = i23 * 10 / (i15 * j);
      
      localGraphics2D.setColor(Color.BLUE);
      localGraphics2D.drawString("J305Beta/Gamma AVG: " + i24 + "/10 [CPM]", 7 + i3, 28 + i2);
    }
    catch (Exception localException2)
    {
      localException2.printStackTrace();
    }
    try
    {
      boolean bool = ImageIO.write(localBufferedImage, "jpg", new File("gmgraph_" + str6 + "jpg"));
      
      bool = ImageIO.write(localBufferedImage, "jpg", new File("gmgraph_current.jpg"));
    }
    catch (Exception localException3)
    {
      localException3.printStackTrace();
    }
    System.exit(0);
  }
}
