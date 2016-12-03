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

public class BinSelectorView extends RelativeLayout {

    private OnBinSelectListener onBinSelectListener;

    public static interface OnBinSelectListener {
        void onBinSelect(Bin bin);
    }

    private ArrayList<Bin> bins;
    private LinearLayout container;
    private Drawable imageBin;
    private Drawable imageBinSelected;

    public BinSelectorView(Context context) {
        super(context);
        init(null, 0);
    }

    public BinSelectorView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public BinSelectorView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
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

        preloadImages(bins.size());
        hideExtraImages(bins.size());

        for (int i = 0; i < bins.size(); i++) {
            Bin bin = bins.get(i);
            this.container.getChildAt(i).setTag(bin);
        }
    }

    private void init(AttributeSet attrs, int defStyle) {
        inflate(getContext(), R.layout.bin_selector_view, this);

        this.container = (LinearLayout)findViewById(R.id.bin_selector_container);
        this.imageBin = ContextCompat.getDrawable(getContext(), R.drawable.ic_bin);
        this.imageBinSelected = ContextCompat.getDrawable(getContext(), R.drawable.ic_bin_selected);
    }

    private void preloadImages(int numberOfBins) {
        int childCount = this.container.getChildCount();
        if (childCount < numberOfBins) {
            for (int i = childCount; i < numberOfBins; i++) {
                createNewImage();
            }
        }
    }

    private void hideExtraImages(int numberOfBins) {
        int childCount = this.container.getChildCount();
        for (int i = 0; i < childCount; i++) {
            if (i >= numberOfBins) {
                View view = this.container.getChildAt(i);
                view.setVisibility(GONE);
            }
        }
    }

    private void onBinTouch(ImageView imageView) {
        clearSelectedBin();

        Bin bin = (Bin)imageView.getTag();
        imageView.setImageDrawable(imageBinSelected);

        if (this.onBinSelectListener != null) {
            this.onBinSelectListener.onBinSelect(bin);
        }
    }

    private void createNewImage() {
        ImageView image = new ImageView(getContext());
        image.setImageDrawable(this.imageBin);

        LinearLayout.LayoutParams vp = new LinearLayout.LayoutParams(
                LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        image.setLayoutParams(vp);
        image.setMaxHeight(50);
        image.setMaxWidth(50);
        image.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                onBinTouch((ImageView)v);
            }
        });

        LinearLayout container = (LinearLayout)findViewById(R.id.bin_selector_container);
        container.addView(image);
    }

    private void clearSelectedBin() {
        int childCount = this.container.getChildCount();
        for (int i = 0; i < childCount; i++) {
            ImageView iv = (ImageView)this.container.getChildAt(i);
            iv.setImageDrawable(imageBin);
        }
    }
}