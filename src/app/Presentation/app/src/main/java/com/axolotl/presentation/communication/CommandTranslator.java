package com.axolotl.presentation.communication;

import com.axolotl.presentation.model.PackageColour;
import com.axolotl.presentation.model.Repository;

public class CommandTranslator {

    public void translate(String command, Repository repository) throws InvalidCommandException {
        if (command == null || command.isEmpty()) {
            throw new IllegalArgumentException("Command cannot be null or empty.");
        }

        String[] parts = command.split(" ");

        if (parts.length < 2) {
            throw new InvalidCommandException("Command has the wrong format.");
        }

        String commandType = parts[0];

        if (commandType.equals("B:")) {
            int length = Integer.parseInt(parts[1]);
            int width = Integer.parseInt(parts[2]);
            int numberOfLayers = Integer.parseInt(parts[3]);
            int id = Integer.parseInt(parts[4]);
            String destination = parts[5];
            if (destination != null && destination.length() > 0) {
                destination = destination.replace(',', ' ');
            }

            repository.createBin(id, length, width, numberOfLayers, destination);

        } else if (commandType.equals("P:")) {

            int x = Integer.parseInt(parts[1]);
            int y = Integer.parseInt(parts[2]);

            int translatedLength = Integer.parseInt(parts[3]);
            int translatedWidth = Integer.parseInt(parts[4]);
            int translatedHeight = 1;

            int measuredLength = Integer.parseInt(parts[5]);
            int measuredWidth = Integer.parseInt(parts[6]);
            int measuredHeight = Integer.parseInt(parts[7]);

            int colourCode = Integer.parseInt(parts[8]);
            boolean isFragile = "1".equals(parts[9]);

            int layer = Integer.parseInt(parts[10]);
            int binId = Integer.parseInt(parts[11]);

            if (!repository.binExists(binId)) {
                throw new InvalidCommandException("Unknown bin id: " + binId);
            }

            if (!PackageColour.exists(colourCode)) {
                throw new InvalidCommandException("Unknown colour code: " + colourCode);
            }

            repository.packPackage(x, y,
                    translatedLength, translatedWidth, translatedHeight,
                    measuredLength, measuredWidth, measuredHeight,
                    colourCode, isFragile, layer, binId);

        } else {
            throw new InvalidCommandException("Unknown command: " + commandType);
        }
    }
}
