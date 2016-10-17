package beuchert.bluetoothtest.Views;

import java.util.Random;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

public class DrawingView extends View {
    int i = 0;
    Bitmap frame ;
    Canvas frameDrawer;
    Rect bounds;
    Paint borderPaint ;
    Random random ;
    int width , height;

    public DrawingView(Context context, int width, int height) {
        super(context);

        this.width = width;
        this.height = height;

        frame = Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
        frameDrawer = new Canvas(frame);
        bounds = new Rect(0 , 0, width,height);
        //this initialization will make the frameDrawer draw on the frame bitmap

        //always avoid allocating new objects in the draw method to optimize the performance 
        borderPaint = new Paint();
        random = new Random();
        borderPaint.setStyle(Paint.Style.STROKE);
        borderPaint.setColor(Color.BLACK);
        Rect r = new Rect(0, 0, width-1, height-1);
        frameDrawer.drawRect(r, borderPaint);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        borderPaint.setColor(Color.argb(255, random.nextInt(255),
                random.nextInt(255), random.nextInt(255)));
        //instead of drawing on the view canvas draw on the frame bitmap
        //frameDrawer.drawLine(random.nextInt(width), random.nextInt(height),
        //        random.nextInt(width), random.nextInt(height), paint);
        // and then draw the bitmap on the view canvas
        canvas.drawBitmap(frame, null, bounds , null);

        //if (i < 1000) {
        //    i++;
        //    invalidate();
        //}
    }

    public void drawRectangle(int x, int y, int length, int width, int paintColor){
        Rect rBorder = new Rect(x, y, x+length, y+width);
        frameDrawer.drawRect(rBorder,borderPaint);

        Paint color = new Paint();
        borderPaint.setStyle(Paint.Style.FILL);

        switch(paintColor){
            case 0:
                color.setColor(Color.RED);
                break;
            case 1:
                color.setColor(Color.GREEN);
                break;
            case 2:
                color.setColor(Color.BLUE);
                break;
            case 3:
                color.setColor(Color.YELLOW);
                break;
        }
        Rect rFill = new Rect(x+1, y+1, x+length-1, y+width-1);
        frameDrawer.drawRect(rFill,color);

        invalidate();

    }
}
