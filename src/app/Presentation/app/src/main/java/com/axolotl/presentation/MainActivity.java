package com.axolotl.presentation;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.CommandTranslator;
import com.axolotl.presentation.model.InvalidCommandException;
import com.axolotl.presentation.model.Repository;

public class MainActivity extends AppCompatActivity implements PackageSelectionChangedEventListener {

    private Repository repository;
    private TwoDimensionalLayerView layerView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        repository = new Repository();


        CommandTranslator cm = new CommandTranslator();
        try {
            // Define bin 1
            cm.translate("B: 10 8 3 1 Foreign", repository);

            // Pack packages into layer 1
            cm.translate("P: 0 0 2 2 1 0 0 1 1", repository);
            cm.translate("P: 3 2 2 4 1 1 0 1 1", repository);

            // Define bin 2
            cm.translate("B: 10 8 3 2 Foreign", repository);

            // Pack packages into layer 1
            cm.translate("P: 1 1 2 2 1 0 0 1 2", repository);
            cm.translate("P: 4 2 2 4 1 1 0 1 2", repository);

            // Define bin 2
            cm.translate("B: 10 8 3 3 Foreign", repository);

            // Pack packages into layer 1
            cm.translate("P: 5 2 2 2 1 0 0 1 3", repository);
            cm.translate("P: 3 0 2 4 1 1 0 1 3", repository);

            // Define bin 2
            cm.translate("B: 10 8 3 4 Foreign", repository);

            // Pack packages into layer 1
            cm.translate("P: 1 1 2 2 1 0 0 1 4", repository);
            cm.translate("P: 4 0 2 4 1 1 0 2 4", repository);

            cm.translate("B: 10 8 3 5 Foreign", repository);
            cm.translate("P: 1 1 2 2 1 0 0 1 5", repository);
            cm.translate("P: 4 0 2 4 1 1 0 2 5", repository);

            cm.translate("B: 10 8 3 6 Foreign", repository);
            cm.translate("P: 1 1 2 2 1 0 0 1 6", repository);
            cm.translate("P: 4 0 2 4 1 1 0 1 6", repository);
            cm.translate("P: 2 5 2 3 1 2 0 1 6", repository);

            cm.translate("B: 10 8 3 7 Foreign", repository);
            cm.translate("P: 1 1 2 2 1 0 0 1 7", repository);
            cm.translate("P: 4 0 2 4 1 1 0 1 7", repository);
        } catch (InvalidCommandException e) {
            e.printStackTrace();
        }

        layerView = (TwoDimensionalLayerView) findViewById(R.id.drawingView);
        layerView.setLayer(repository.getCurrentLayer());
        layerView.setPackageSelectionChangedEventListener(this);

        BinSelectorView binSelectorView = (BinSelectorView)findViewById(R.id.bin_selector);
        binSelectorView.setBins(repository.getBins());
        binSelectorView.setBinSelectListener(new BinSelectorView.OnBinSelectListener() {
            @Override
            public void onBinSelect(Bin bin) {
                repository.selectBin(bin);
                layerView.setLayer(repository.getCurrentLayer());
            }
        });

        LayerSelectorView layerSelector = (LayerSelectorView)findViewById(R.id.layer_selector);
        layerSelector.setBin(repository.getSelectedBin());
    }

    @Override
    public void onPackageSelectionChanged() {
        Log.d("MainActivity", "Package selection changed.");
    }
}
