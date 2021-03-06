package com.axolotl.presentation.views;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.axolotl.presentation.R;
import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.PackageDimension;
import com.axolotl.presentation.model.PackedPackage;

public class LayerView extends View {
    public interface OnPackageSelectListener {
        void onPackageSelect(Package thePackage);
    }

    private final Paint strokePaint;
    private final Paint fillPaint;
    private final Rect rect;
    private final Drawable packageSelectedIcon;
    private Layer layer;
    private Package selectedPackage;
    private OnPackageSelectListener packageSelectListener;
    private final int PADDING_TOP = 10;
    private final int PADDING_LEFT = 10;
    private final int PADDING_RIGHT = 10;

    public LayerView(Context context, AttributeSet attrs) {
        super(context, attrs);

        // Initialisation
        strokePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        strokePaint.setStyle(Paint.Style.STROKE);
        strokePaint.setColor(Color.BLACK);

        fillPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        fillPaint.setStyle(Paint.Style.FILL);
        fillPaint.setColor(Color.GRAY);

        rect = new Rect();

        this.packageSelectedIcon = ContextCompat.getDrawable(getContext(), R.drawable.ic_package_selected);

        setBackgroundColor(Color.WHITE);
    }

    public void setData(Layer layer, Package selectedPackage){
        this.layer = layer;
        this.selectedPackage = selectedPackage;
        invalidate();
        requestLayout();
    }

    public void setPackageSelectListener(OnPackageSelectListener packageSelectListener) {
        this.packageSelectListener = packageSelectListener;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (this.layer != null) {
            int scaleFactor = calcScaleFactor(this.layer);
            int x = (int)event.getX() / scaleFactor;
            int y = (int)event.getY() / scaleFactor;
            Package aPackage = this.layer.select(x, y);
            if (aPackage != null && this.packageSelectListener != null) {
                this.packageSelectListener.onPackageSelect(aPackage);
            }
            invalidate();
            requestLayout();
        }

        return super.onTouchEvent(event);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (layer == null) {
            drawEmpty(canvas);
        } else {
            drawLayer(canvas);
        }
    }

    private void drawLayer(Canvas canvas) {
        int scaleFactor = calcScaleFactor(layer);
        int width = layer.getLength() * scaleFactor;
        int height = layer.getWidth() * scaleFactor;

        //canvas.drawRect(0, 0, width-1, height-1, strokePaint);

        for (int i = 0; i < layer.getNumberOfPackages(); i++) {
            PackedPackage p = layer.get(i);
            drawPackage(canvas, p, i, scaleFactor);
        }

        //drawDots(canvas, scaleFactor, layer.getLength(), layer.getWidth());
    }

    private void drawPackage(Canvas canvas, PackedPackage packagedPackage, int index, int scaleFactor) {
        PackageDimension pd = packagedPackage.getPackage().getDimension();

        int left = packagedPackage.getX() * scaleFactor;
        int top = packagedPackage.getY() * scaleFactor;
        int right = left + pd.getLength() * scaleFactor;
        int bottom = top + pd.getWidth() * scaleFactor;

        int argbColour = ColourMapping.getPaintColour(packagedPackage.getPackage().getColour());
        fillPaint.setColor(argbColour);

        canvas.drawRect(left, top, right, bottom, fillPaint);
        canvas.drawRect(left, top, right, bottom, strokePaint);

        //drawLegoDots(canvas, packagedPackage, scaleFactor);

        String number = Integer.toString(packagedPackage.getPackage().getNumber());

        fillPaint.setColor(Color.BLACK);
        fillPaint.setTextSize(20);
        canvas.drawText(number, left + PADDING_LEFT, top + 25, fillPaint);

        if (selectedPackage != null && selectedPackage == packagedPackage.getPackage()) {
            drawSelectedIcon(canvas, this.packageSelectedIcon, left, top, right, bottom);
        }
    }

    private void drawLegoDots(Canvas canvas, PackedPackage packagedPackage, int scaleFactor) {
        float x = packagedPackage.getX() * scaleFactor;
        float y = packagedPackage.getY() * scaleFactor;
        PackageDimension pd = packagedPackage.getPackage().getDimension();

        float radius = scaleFactor / 3;
        x = x + radius + (radius/2);
        y = y+radius + (radius/2);
        canvas.drawCircle(x, y, radius, strokePaint);

        x = x + (radius/2) + radius;
        y = y;
        canvas.drawCircle(x, y, radius, strokePaint);
    }

    private void drawSelectedIcon(Canvas canvas, Drawable icon, int left, int top, int right, int bottom) {
        int iconWidth = icon.getIntrinsicWidth();
        int iconHeight = icon.getIntrinsicHeight();

        //int newLeft = ((right - left) / 2) + left - (iconWidth / 2);
        //int newTop = ((bottom - top) / 2) + top - (iconHeight / 2);
        int newLeft = right - iconWidth - PADDING_RIGHT;
        int newTop = top + PADDING_TOP;

        icon.setBounds(newLeft, newTop, newLeft+iconWidth, newTop+iconHeight);
        icon.draw(canvas);
    }

    private void drawDots(Canvas canvas, int scaleFactor, int numberOfHorisontalDots, int numberOfVerticalDots) {
        for (int i = 0; i < numberOfHorisontalDots; i++) {
            for (int j = 0; j < numberOfVerticalDots; j++) {
                float left = i * scaleFactor - 1;
                float top = j * scaleFactor - 1;
                float right = left + 2;
                float bottom = top + 2;

                canvas.drawRect(left, top, right, bottom, strokePaint);
            }
        }

    }

    private void drawEmpty(Canvas canvas) {
        fillPaint.setColor(Color.BLACK);
        fillPaint.setTextSize(50);

        String text = "No layer selected.";

        // Source: http://stackoverflow.com/questions/11120392/android-center-text-on-canvas
        canvas.getClipBounds(rect);
        int cHeight = rect.height();
        int cWidth = rect.width();
        fillPaint.setTextAlign(Paint.Align.LEFT);
        fillPaint.getTextBounds(text, 0, text.length(), rect);
        float x = cWidth / 2f - rect.width() / 2f - rect.left;
        float y = cHeight / 2f + rect.height() / 2f - rect.bottom;
        canvas.drawText(text, x, y, fillPaint);
    }

    private int calcScaleFactor(Layer theLayer) {
        return Math.round((float)getMeasuredWidth() / (float)Math.max(theLayer.getLength(), theLayer.getWidth()));
    }
}