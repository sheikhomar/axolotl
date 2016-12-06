package com.axolotl.presentation;

import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.PackageColour;
import com.axolotl.presentation.model.PackageDimension;
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

            repository.createBin(id, length, width, numberOfLayers, destination);

        } else if (commandType.equals("P:")) {
            int x = Integer.parseInt(parts[1]);
            int y = Integer.parseInt(parts[2]);

            int length = Integer.parseInt(parts[3]);
            int width = Integer.parseInt(parts[4]);
            int height = 1;

            int originalLength = Integer.parseInt(parts[5]);
            int originalWidth = Integer.parseInt(parts[6]);
            int originalHeight = Integer.parseInt(parts[7]);

            int colourCode = Integer.parseInt(parts[8]);
            boolean isFragile = parts[9] == "1";
            int layer = Integer.parseInt(parts[10]);
            int binId = Integer.parseInt(parts[11]);

            Bin bin = repository.getBinById(binId);
            if (bin == null) {
                throw new InvalidCommandException("Unknown bin id: " + binId);
            }

            PackageDimension translated = new PackageDimension(length, width, height);
            PackageDimension real = new PackageDimension(originalLength, originalWidth, originalHeight);
            Package p = new Package(translated, real, PackageColour.parse(colourCode), isFragile);

            bin.pack(p, layer, x, y);
        } else {
            throw new InvalidCommandException("Unknown command: " + commandType);
        }
    }
}
