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
import beuchert.bluetoothtest.Models.Bin;
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
    List<Bin> bins;
    int selectedLayer = 0;
    private int selectedBin = 0;

    public DrawingView(Context context, int length, int width) {
        super(context);

        mainActivity = (MainActivity) context;
        this.length = length;
        this.width = width;

        bins = new ArrayList<Bin>();

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
        canvas.drawBitmap(frame, null, bounds , null);
    }

    public void addBin(Bin bin){
        bins.add(bin);
        int index = bins.indexOf(bin);

        for(int i = 0; i < bin.amountOfLayers; i++){
            bin.addLayerToBin(new Layer());
            mainActivity.addElementToSpinner("Bin: " + (index+1) + " Layer: " + (i+1));
        }
    }

    public void addPackage(Package pack){
        Bin bin = null;
        Layer layer = null;

        bin = bins.get(pack.bin-1);

        layer = bin.layers.get(pack.layer-1);

        layer.addPackageToLayer(pack);

        selectBinAndLayer(pack.bin, pack.layer);
    }

    private void drawSize(){
        frameDrawer.drawColor(Color.WHITE);

        Rect r = new Rect(0, 0, width-1, length-1);
        frameDrawer.drawRect(r, borderPaint);

        frameDrawer.drawBitmap(frame, null, bounds , null);

        invalidate();
    }

    private void redrawSelectedBinAndLayer(){
        frameDrawer.drawColor(Color.WHITE);

        Rect r = new Rect(0, 0, width-1, length-1);
        frameDrawer.drawRect(r, borderPaint);

        Bin bin = bins.get(selectedBin-1);

        Layer layer = bin.layers.get(selectedLayer-1);
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


    public void selectBinAndLayer(int bin, int layer){
        selectedBin = bin;
        selectedLayer = layer;
        mainActivity.setSelectedElementInSpinner(selectedBin, selectedLayer);
        redrawSelectedBinAndLayer();
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
        drawSize();
    }

    public void shiftRight(){
        if(bins.size() != 0){
            if(selectedLayer != bins.get(selectedBin-1).layers.size()){
                selectedLayer = selectedLayer+1;
                selectBinAndLayer(selectedBin, selectedLayer);
            }
            else{
                if(selectedBin != bins.size()){
                    selectedBin = selectedBin + 1;
                    selectedLayer = 1;
                }
            }

            selectBinAndLayer(selectedBin, selectedLayer);
            mainActivity.setSelectedElementInSpinner(selectedBin, selectedLayer);
        }
    }

    public void shiftLeft(){
        if(bins.size() != 0){
            if(selectedLayer != 1){
                selectedLayer = selectedLayer-1;
                selectBinAndLayer(selectedBin, selectedLayer);
            }
            else{
                if(selectedBin != 1){
                    selectedBin = selectedBin - 1;
                    selectedLayer = 3;
                }
            }

            selectBinAndLayer(selectedBin, selectedLayer);
            mainActivity.setSelectedElementInSpinner(selectedBin, selectedLayer);
        }
    }
}
