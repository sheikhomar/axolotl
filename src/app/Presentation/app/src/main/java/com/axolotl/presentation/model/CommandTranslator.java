package com.axolotl.presentation.model;

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
            int height = Integer.parseInt(parts[5]);

            int colourCode = Integer.parseInt(parts[6]);
            boolean isFragile = parts[7] == "1";
            int layer = Integer.parseInt(parts[8]);
            int binId = Integer.parseInt(parts[9]);

            Bin bin = repository.getBinById(binId);
            if (bin == null) {
                throw new InvalidCommandException("Unknown bin id: " + binId);
            }

            PackageDimension pd = new PackageDimension(length, width, height);
            Package p = new Package(pd, pd, PackageColour.parse(colourCode), isFragile);

            bin.pack(p, layer, x, y);
        } else {
            throw new InvalidCommandException("Unknown command: " + commandType);
        }
    }
}
