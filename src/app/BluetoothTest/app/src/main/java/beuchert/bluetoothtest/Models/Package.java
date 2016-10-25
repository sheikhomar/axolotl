package beuchert.bluetoothtest.Models;

import android.graphics.Color;
import android.graphics.Paint;

public class Package {
    public int width;
    public int length;
    private int color;
    public int x;
    public int y;
    public Paint paint;
    public int layer;
    public int bin;

    public Package(int startX, int startY, int packLength, int packWidth, int paintColor, int layerNumber, int binNumber){
        x = startX;
        y = startY;
        length = packLength;
        width = packWidth;
        layer = layerNumber;
        bin = binNumber;

        paint = new Paint();
        paint.setStyle(Paint.Style.FILL);

        switch(paintColor){
            case 0:
                paint.setColor(Color.RED);
                break;
            case 1:
                paint.setColor(Color.GREEN);
                break;
            case 2:
                paint.setColor(Color.BLUE);
                break;
            case 3:
                paint.setColor(Color.YELLOW);
                break;
            default:
                paint.setColor(Color.WHITE);
                break;
        }
    }
}
