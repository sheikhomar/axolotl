package com.axolotl.presentation;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.Repository;

public class MainActivity extends AppCompatActivity {

    private Repository repository;
    private TwoDimensionalLayerView layerView;
    private BinSelectorView binSelector;
    private LayerSelectorView layerSelector;
    private PackageDetailsView packageDetailsView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        repository = ((App)getApplication()).getRepository();

        this.layerView = (TwoDimensionalLayerView) findViewById(R.id.layer_view);
        this.layerSelector = (LayerSelectorView)findViewById(R.id.layer_selector);
        this.binSelector = (BinSelectorView)findViewById(R.id.bin_selector);
        this.packageDetailsView = (PackageDetailsView)findViewById(R.id.package_details);

        this.binSelector.setBins(repository.getBins());
        this.binSelector.setBinSelectListener(new BinSelectorView.OnBinSelectListener() {
            @Override
            public void onBinSelect(Bin bin) {
                repository.selectBin(bin);
                layerSelector.setBin(bin);
                layerView.setLayer(repository.getSelectedLayer());
            }
        });

        this.layerSelector.setBin(repository.getSelectedBin());
        this.layerSelector.setLayerSelectListener(new LayerSelectorView.OnLayerSelectListener() {
            @Override
            public void onLayerSelect(Layer layer) {
                repository.selectLater(layer);
                layerView.setLayer(repository.getSelectedLayer());
            }
        });

        this.layerView.setLayer(repository.getSelectedLayer());
        this.layerView.setPackageSelectListener(new TwoDimensionalLayerView.OnPackageSelectListener() {
            @Override
            public void onPackageSelect(Package aPackage) {
                packageDetailsView.setPackage(aPackage);
            }
        });
    }
}