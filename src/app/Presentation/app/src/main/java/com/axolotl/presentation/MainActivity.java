package com.axolotl.presentation;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.CommandTranslator;
import com.axolotl.presentation.model.InvalidCommandException;
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

        repository = new Repository();

        populateRepostioryWithTestData();

        this.layerView = (TwoDimensionalLayerView) findViewById(R.id.layer_view);
        this.layerSelector = (LayerSelectorView)findViewById(R.id.layer_selector);
        this.binSelector = (BinSelectorView)findViewById(R.id.bin_selector);
        this.packageDetailsView = (PackageDetailsView)findViewById(R.id.package_details);

        this.layerView.setLayer(repository.getSelectedLayer());
        this.layerView.setPackageSelectListener(new TwoDimensionalLayerView.OnPackageSelectListener() {
            @Override
            public void onPackageSelect(Package aPackage) {
                packageDetailsView.setPackage(aPackage);
            }
        });

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
    }

    private void populateRepostioryWithTestData() {
        CommandTranslator cm = new CommandTranslator();
        try {
            // Define bin 1
            cm.translate("B: 10 8 3 1 Foreign", repository);

            // Pack packages into layer 1
            cm.translate("P: 0 0 2 2 1 0 0 1 1", repository);
            cm.translate("P: 2 0 2 2 1 1 0 1 1", repository);
            cm.translate("P: 4 0 2 2 1 2 0 1 1", repository);
            cm.translate("P: 6 0 2 2 1 3 0 1 1", repository);
            cm.translate("P: 0 2 2 2 1 6 0 1 1", repository);
            cm.translate("P: 2 2 2 2 1 7 0 1 1", repository);
            cm.translate("P: 4 2 2 2 1 128 0 1 1", repository);

            // Pack packages into layer 2
            cm.translate("P: 0 0 2 8 1 0 0 2 1", repository);
            cm.translate("P: 2 0 2 4 1 1 0 2 1", repository);
            cm.translate("P: 4 0 2 2 1 2 0 2 1", repository);

            // Pack packages into layer 3
            cm.translate("P: 0 0 2 8 1 2 0 3 1", repository);
            cm.translate("P: 2 0 2 4 1 2 0 3 1", repository);
            cm.translate("P: 4 0 2 2 1 2 0 3 1", repository);

            // Define bin 2
            cm.translate("B: 10 8 2 2 Foreign", repository);

            // Pack packages into layer 1
            cm.translate("P: 2 1 2 2 1 0 0 1 2", repository);
            cm.translate("P: 3 3 2 2 1 1 0 1 2", repository);
            cm.translate("P: 6 0 2 2 1 2 0 1 2", repository);

            // Pack packages into layer 2
            cm.translate("P: 0 0 2 8 1 0 0 2 2", repository);
            cm.translate("P: 2 0 2 4 1 1 0 2 2", repository);


        } catch (InvalidCommandException e) {
            e.printStackTrace();
        }
    }
}
