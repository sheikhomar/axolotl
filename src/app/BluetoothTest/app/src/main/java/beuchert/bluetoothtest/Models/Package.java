package beuchert.bluetoothtest.Models;

import android.graphics.Color;
import android.graphics.Paint;

public class Package {
    public int width;
    public int length;
    public int height;
    private int colour;
    public boolean fragile;
    public int x;
    public int y;
    public Paint paint;
    public int layer;
    public int bin;

    public Package(int startX, int startY, int packLength, int packWidth, int packHeight, int paintColor, int packFragile, int layerNumber, int binNumber){
        x = startX;
        y = startY;
        length = packLength;
        width = packWidth;
        height = packHeight;
        layer = layerNumber;
        bin = binNumber;
        colour = paintColor;

        if(packFragile == 0){
            fragile = false;
        }
        else{
            fragile = true;
        }

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

    public String getColour(){
        switch(colour){
            case 0: return "Red";
            case 1: return "Green";
            case 2: return "Blue";
            case 3: return "Yellow";
            default: return "Unknown";
        }
    }
}
