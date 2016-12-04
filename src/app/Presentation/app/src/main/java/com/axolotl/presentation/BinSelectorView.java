package com.axolotl.presentation;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.axolotl.presentation.model.Bin;

import java.util.ArrayList;

public class BinSelectorView extends BaseSelectorView {

    private OnBinSelectListener onBinSelectListener;

    public interface OnBinSelectListener {
        void onBinSelect(Bin bin);
    }

    private ArrayList<Bin> bins;

    public BinSelectorView(Context context) {
        super(context);
    }

    public BinSelectorView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public BinSelectorView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setBins(ArrayList<Bin> bins) {
        this.bins = bins;
        buildImages();
    }

    public void setBinSelectListener(OnBinSelectListener onBinSelectListener) {
        this.onBinSelectListener = onBinSelectListener;
    }

    private void buildImages() {
        if (bins == null)
            return;

        showIcons(bins.size());

        for (int i = 0; i < bins.size(); i++) {
            Bin bin = bins.get(i);
            this.container.getChildAt(i).setTag(bin);
        }
    }

    @Override
    protected int getNormalIconResource() { return R.drawable.ic_bin; }

    @Override
    protected int getSelectedIconResource() { return R.drawable.ic_bin_selected; }

    @Override
    protected int getResourceView() { return R.layout.bin_selector_view; }

    @Override
    protected void onIconClick(ImageView view) {
        clearSelection();

        Bin bin = (Bin)view.getTag();
        view.setImageDrawable(this.selectedIcon);

        if (this.onBinSelectListener != null) {
            this.onBinSelectListener.onBinSelect(bin);
        }
    }
}