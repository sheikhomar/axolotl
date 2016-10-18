package beuchert.bluetoothtest.Views;

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;
import java.util.Random;
import java.util.concurrent.TimeUnit;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import beuchert.bluetoothtest.Activities.MainActivity;
import beuchert.bluetoothtest.Models.Layer;
import beuchert.bluetoothtest.Models.Package;
import beuchert.bluetoothtest.R;

public class DrawingView extends View {
    int i = 0;
    Bitmap frame ;
    Canvas frameDrawer;
    Rect bounds;
    Paint borderPaint ;
    Random random ;
    MainActivity mainActivity;
    int width , length;
    List<Layer> layers;
    int selectedLayer = 0;

    public DrawingView(Context context, int length, int width) {
        super(context);

        mainActivity = (MainActivity) context;
        this.length = length;
        this.width = width;

        layers = new ArrayList<Layer>();

        frame = Bitmap.createBitmap(width,length,Bitmap.Config.ARGB_8888);
        frameDrawer = new Canvas(frame);
        bounds = new Rect(0 , 0, width,length);

        borderPaint = new Paint();
        random = new Random();
        borderPaint.setStyle(Paint.Style.STROKE);
        borderPaint.setColor(Color.BLACK);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        Rect r = new Rect(0, 0, width-1, length-1);
        frameDrawer.drawRect(r, borderPaint);

        canvas.drawBitmap(frame, null, bounds , null);
    }

    public void addPackage(Package pack){
        Layer layer = null;

        if (layers.size()-1 < pack.layer){
            while(layers.size()-1 < pack.layer){
                layers.add(new Layer());
                if (layers.size()-1 == pack.layer){
                    layer = layers.get(pack.layer);
                    mainActivity.addElementToSpinner("Layer " + pack.layer);
                }
            }
        }
        else
            layer = layers.get(pack.layer);

        layer.addPackageToLayer(pack);

        selectLayer(pack.layer);

        redrawSelectedLayer();
    }

    private void redrawSelectedLayer(){
        frameDrawer.drawColor(Color.WHITE);

        Rect r = new Rect(0, 0, width-1, length-1);
        frameDrawer.drawRect(r, borderPaint);

        Layer layer = layers.get(selectedLayer);
        List<Package> packList = layer.Packages;

        for (Package pack:packList) {
            Rect rBorder = new Rect(pack.x, pack.y, pack.x+pack.length, pack.y+pack.width);
            frameDrawer.drawRect(rBorder,borderPaint);

            Rect rFill = new Rect(pack.x+1, pack.y+1, pack.x+pack.length, pack.y+pack.width);
            frameDrawer.drawRect(rFill,pack.paint);
        }

        frameDrawer.drawBitmap(frame, null, bounds , null);

        invalidate();
    }

    public void selectLayer(int selector){
        selectedLayer = selector;
        mainActivity.setSelectedElementInSpinner(selector-1);
        redrawSelectedLayer();
    }

    public void setSize(int length, int width){
        frameDrawer.drawColor(Color.WHITE);
        this.length = length;
        this.width = width;

        frame = Bitmap.createBitmap(width,length,Bitmap.Config.ARGB_8888);
        frameDrawer = new Canvas(frame);
        bounds = new Rect(0 , 0, width,length);

        Rect r = new Rect(0, 0, width-1, length-1);
        frameDrawer.drawRect(r, borderPaint);

        invalidate();
        redrawSelectedLayer();
    }

}
