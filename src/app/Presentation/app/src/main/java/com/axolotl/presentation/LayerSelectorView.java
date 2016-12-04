package com.axolotl.presentation;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.ImageView;

import com.axolotl.presentation.model.Bin;

public class LayerSelectorView extends BaseSelectorView {
    private Bin bin;

    public LayerSelectorView(Context context) {
        super(context);
    }

    public LayerSelectorView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public LayerSelectorView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected int getNormalIconResource() { return R.drawable.ic_layer; }

    @Override
    protected int getSelectedIconResource() { return R.drawable.ic_layer_selected; }

    @Override
    protected int getResourceView() { return R.layout.layer_selector_view; }

    @Override
    protected void onIconClick(ImageView view) {

    }

    public void setBin(Bin bin) {
        this.bin = bin;

        if (bin != null) {
            showIcons(bin.getNumberOfLayers());
            for (int i = 0; i < bin.getNumberOfLayers(); i++) {
                this.container.getChildAt(i).setTag(i);
            }
        }
    }
}