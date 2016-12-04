package com.axolotl.presentation;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.PackageDimension;
import com.axolotl.presentation.model.PackagedPackage;

public class TwoDimensionalLayerView extends View {

    private final Paint strokePaint;
    private final Paint fillPaint;
    private final Paint boldPaint;
    private final Drawable packageSelectedIcon;
    private Layer layer;
    private PackageSelectionChangedEventListener pscEventListener;


    public TwoDimensionalLayerView(Context context, AttributeSet attrs) {
        super(context, attrs);

        // Initialisation
        strokePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        strokePaint.setStyle(Paint.Style.STROKE);
        strokePaint.setColor(Color.BLACK);

        fillPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        fillPaint.setStyle(Paint.Style.FILL);
        fillPaint.setColor(Color.GRAY);

        boldPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        boldPaint.setStyle(Paint.Style.STROKE);
        boldPaint.setColor(Color.GREEN);

        this.packageSelectedIcon = ContextCompat.getDrawable(getContext(), R.drawable.ic_package_selected);

        setBackgroundColor(Color.WHITE);
    }

    public void setLayer(Layer layer){
        this.layer = layer;
        invalidate();
        requestLayout();
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

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (this.layer != null) {
            int scaleFactor = calcScaleFactor(this.layer);
            int x = (int)event.getX() / scaleFactor;
            int y = (int)event.getY() / scaleFactor;
            this.layer.select(x, y);
            firePackageSelectionChangedEvent();
            invalidate();
            requestLayout();
        }

        return super.onTouchEvent(event);
    }

    private void drawLayer(Canvas canvas) {
        int scaleFactor = calcScaleFactor(layer);
        int width = layer.getLength() * scaleFactor;
        int height = layer.getWidth() * scaleFactor;

        canvas.drawRect(0, 0, width-1, height-1, boldPaint);

        for (int i = 0; i < layer.getNumberOfPackages(); i++) {
            PackagedPackage p = layer.get(i);
            drawPackage(canvas, p, i, scaleFactor);
        }

        drawDots(canvas, scaleFactor, layer.getLength(), layer.getWidth());
    }

    public void setPackageSelectionChangedEventListener(PackageSelectionChangedEventListener eventListener) {
        this.pscEventListener = eventListener;
    }

    private void drawPackage(Canvas canvas, PackagedPackage packagedPackage, int index, int scaleFactor) {
        PackageDimension pd = packagedPackage.getPackage().getDimension();

        int left = packagedPackage.getX() * scaleFactor;
        int top = packagedPackage.getY() * scaleFactor;
        int right = left + pd.getLength() * scaleFactor;
        int bottom = top + pd.getWidth() * scaleFactor;

        fillPaint.setColor(packagedPackage.getPackage().getColour().getPaintColour());

        canvas.drawRect(left, top, right, bottom, fillPaint);
        canvas.drawRect(left, top, right, bottom, strokePaint);

        if (layer.isSelected(index)) {
            drawIcon(canvas, this.packageSelectedIcon, left, top, right, bottom);
        }
    }

    private void drawIcon(Canvas canvas, Drawable icon, int left, int top, int right, int bottom) {
        int iconWidth = icon.getIntrinsicWidth();
        int iconHeight = icon.getIntrinsicHeight();

        int newLeft = ((right - left) / 2) + left - (iconWidth / 2);
        int newTop = ((bottom - top) / 2) + top - (iconHeight / 2);

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
        float xpad = (float)(getPaddingLeft() + getPaddingRight());
        float ypad = (float)(getPaddingTop() + getPaddingBottom());

        float top = canvas.getClipBounds().top;
        float left = canvas.getClipBounds().left;
        float right = canvas.getClipBounds().right;
        float bottom = canvas.getClipBounds().bottom;

        float height = canvas.getHeight();
        float width = canvas.getWidth();

        canvas.drawLine(width/2, 0, width/2, height - 10, boldPaint);

        canvas.drawRect(top, left, right, bottom, strokePaint);
        canvas.drawLine(xpad, ypad, 1000, 1000, strokePaint);
    }

    private int calcScaleFactor(Layer theLayer) {
        return Math.round((float)getMeasuredWidth() / (float)Math.max(theLayer.getLength(), theLayer.getWidth()));
    }

    private void firePackageSelectionChangedEvent() {
        if (this.pscEventListener != null) {
            this.pscEventListener.onPackageSelectionChanged();
        }
    }
}