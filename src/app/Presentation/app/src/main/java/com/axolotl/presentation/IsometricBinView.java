package com.axolotl.presentation;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.view.View;

public class IsometricBinView extends View {
    private final Paint strokePaint;
    private final Paint fillPaint;
    private final Paint boldPaint;

    public IsometricBinView(Context context, AttributeSet attrs) {
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
    }

    @Override
    protected void onDraw(Canvas canvas) {
        int scaleFactor = 50;
        int height = 2 * scaleFactor;
        int width = 2 * scaleFactor;
        int startX = 100;
        int startY = 100;

        Drawable drawable = ContextCompat.getDrawable(getContext(),R.drawable.ic_bin);
        drawable.draw(canvas);



        canvas.drawLine(startX, startY, startX, startY + height, strokePaint);
        startX = startX;
        startY = startY + height;

        canvas.drawLine(startX, startY, startX - scaleFactor, startY - scaleFactor, strokePaint);
        startX = startX - scaleFactor;
        startY = startY - scaleFactor;

        canvas.drawLine(startX, startY, startX, startY - height, strokePaint);
        startX = startX;
        startY = startY - height;

        canvas.drawLine(startX, startY, startX + scaleFactor, startY + scaleFactor, strokePaint);
        startX = startX + scaleFactor;
        startY = startY + scaleFactor;

        canvas.drawLine(startX, startY, startX + width, startY, strokePaint);
        startX = startX;
        startY = startY + height;

        canvas.drawLine(startX, startY, startX + width, startY, strokePaint);
        startX = startX;
        startY = startY + height;



    }
}
