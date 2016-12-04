package com.axolotl.presentation;

import android.app.Application;

import com.axolotl.presentation.model.Repository;

public class App extends Application {

    private Repository repository;

    public App() {
        repository = new Repository();
        populateRepostioryWithTestData();
    }

    public Repository getRepository() {
        return repository;
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
