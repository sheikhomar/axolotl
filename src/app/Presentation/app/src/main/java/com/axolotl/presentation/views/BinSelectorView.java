package com.axolotl.presentation.views;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.widget.ImageView;

import com.axolotl.presentation.R;
import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.PackageColour;

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

    public void setData(ArrayList<Bin> bins, Bin selectedBin) {
        this.bins = bins;

        if (this.bins == null)
            return;

        showIcons(this.bins.size());

        int selectedIndex = 0;
        for (int i = 0; i < this.bins.size(); i++) {
            Bin bin = bins.get(i);
            this.container.getChildAt(i).setTag(bin);
            if (bin == selectedBin) {
                selectedIndex = i;
            }
        }

        this.clearSelection();
        this.highlightIcon(selectedIndex);
    }

    public void setBinSelectListener(OnBinSelectListener onBinSelectListener) {
        this.onBinSelectListener = onBinSelectListener;
    }

    @Override
    protected int getNormalIconResource() { return R.drawable.ic_bin; }

    @Override
    protected int getSelectedIconResource() { return R.drawable.ic_bin_selected; }

    @Override
    protected int getResourceView() { return R.layout.bin_selector_view; }

    @Override
    protected void onIconClick(ImageView view) {
        Bin bin = (Bin)view.getTag();

        if (this.onBinSelectListener != null) {
            this.onBinSelectListener.onBinSelect(bin);
        }
    }
}