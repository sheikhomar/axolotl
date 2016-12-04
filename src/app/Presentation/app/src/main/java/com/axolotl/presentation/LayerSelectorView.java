package com.axolotl.presentation;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ImageView;

import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.Layer;

public class LayerSelectorView extends BaseSelectorView {
    public interface OnLayerSelectListener {
        void onLayerSelect(Layer layer);
    }

    private Bin bin;
    private OnLayerSelectListener layerSelectListener;

    public LayerSelectorView(Context context) {
        super(context);
    }

    public LayerSelectorView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public LayerSelectorView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setLayerSelectListener(OnLayerSelectListener layerSelectListener) {
        this.layerSelectListener = layerSelectListener;
    }

    public void setBin(Bin bin) {
        this.bin = bin;

        if (bin != null) {
            Log.d("LayerSelectorView", "Bin " + bin.getId() + " has " + bin.getNumberOfLayers() + " layers.");

            showIcons(bin.getNumberOfLayers());
            for (int i = 0; i < bin.getNumberOfLayers(); i++) {
                Layer layer = bin.getLayerAt(i);
                this.container.getChildAt(i).setTag(layer);
            }
        }
    }

    @Override
    protected void onIconClick(ImageView view) {
        if (this.layerSelectListener != null) {
            Layer layer = (Layer)view.getTag();
            this.layerSelectListener.onLayerSelect(layer);
        }
    }

    @Override
    protected int getNormalIconResource() { return R.drawable.ic_layer; }

    @Override
    protected int getSelectedIconResource() { return R.drawable.ic_layer_selected; }

    @Override
    protected int getResourceView() { return R.layout.layer_selector_view; }

}