package com.axolotl.presentation;

import android.app.Application;

import com.axolotl.presentation.communication.CommandParser;
import com.axolotl.presentation.communication.InvalidCommandException;
import com.axolotl.presentation.model.ColourCodeParser;
import com.axolotl.presentation.model.Repository;

public class App extends Application {

    private Repository repository;

    public App() {
        repository = new Repository(new ColourCodeParser());
        //populateRepostioryWithTestData();
    }

    public Repository getRepository() {
        return repository;
    }

    private void populateRepostioryWithTestData() {
        CommandParser cm = new CommandParser();
        try {
            // Define bin 1
            cm.parse("B: 10 8 3 1 Foreign", repository);

            // Pack packages into layer 1
            cm.parse("P: 0 0 2 2 160 160 160 0 0 1 1", repository);
            cm.parse("P: 2 0 2 2 160 160 160 1 0 1 1", repository);
            cm.parse("P: 4 0 2 2 160 160 160 2 0 1 1", repository);
            cm.parse("P: 6 0 2 2 160 160 160 3 0 1 1", repository);
            cm.parse("P: 0 2 2 2 160 160 160 6 0 1 1", repository);
            cm.parse("P: 2 2 2 2 160 160 160 7 0 1 1", repository);
            cm.parse("P: 4 2 2 2 160 160 160 128 0 1 1", repository);

            // Pack packages into layer 2
            cm.parse("P: 0 0 2 8 160 160 160 0 0 2 1", repository);
            cm.parse("P: 2 0 2 4 160 160 160 1 0 2 1", repository);
            cm.parse("P: 4 0 2 2 160 160 160 2 0 2 1", repository);

            // Pack packages into layer 3
            cm.parse("P: 0 0 2 8 160 160 160 2 0 3 1", repository);
            cm.parse("P: 2 0 2 4 160 160 160 2 0 3 1", repository);
            cm.parse("P: 4 0 2 2 160 160 160 2 0 3 1", repository);

            // Define bin 2
            cm.parse("B: 10 8 2 2 Foreign", repository);

            // Pack packages into layer 1
            cm.parse("P: 2 1 2 2 160 160 160 0 0 1 2", repository);
            cm.parse("P: 3 3 2 2 160 160 160 1 0 1 2", repository);
            cm.parse("P: 6 0 2 2 160 160 160 2 0 1 2", repository);

            // Pack packages into layer 2
            cm.parse("P: 0 0 2 8 160 160 160 0 0 2 2", repository);
            cm.parse("P: 2 0 2 4 160 160 160 1 0 2 2", repository);


        } catch (InvalidCommandException e) {
            e.printStackTrace();
        }
    }
}
