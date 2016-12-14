package com.axolotl.presentation.communication;

import android.support.annotation.NonNull;

import com.axolotl.presentation.model.Repository;

public class CommandTranslator {
    public static final String ERR_CMD_UNKNOWN = "Command is unknown";
    public static final String ERR_REPOSITORY_IS_NULL = "Repository cannot be null";
    public static final String ERR_CMD_NULL_OR_EMPTY = "Command cannot be null or empty";
    public static final String ERR_BIN_CMD_WRONG_FORMAT = "Bin command has the wrong format";
    public static final String ERR_BIN_CMD_WRONG_ELEMENTS = "Bin command has the wrong number of elements";
    public static final String ERR_PACKAGE_CMD_WRONG_FORMAT = "Package command has the wrong format";
    public static final String ERR_PACKAGE_CMD_WRONG_ELEMENTS = "Package command has the wrong number of elements";
    public static final String ERR_UNKNOWN_BIN = "Unknown bin id: ";
    public static final String ERR_UNKNOWN_COLOUR = "Unknown colour code: ";

    public void translate(@NonNull String command, @NonNull Repository repository) throws InvalidCommandException {
        if (repository == null) {
            throw new IllegalArgumentException(ERR_REPOSITORY_IS_NULL);
        }
        if (command == null || command.isEmpty()) {
            throw new IllegalArgumentException(ERR_CMD_NULL_OR_EMPTY);
        }

        String[] parts = command.split(" ");

        String commandType = parts[0];

        if (commandType.equals("B:")) {
            parseBinCommand(repository, parts);
        } else if (commandType.equals("P:")) {
            parsePackageCommand(repository, parts);
        } else {
            throw new InvalidCommandException(ERR_CMD_UNKNOWN);
        }
    }

    private void parsePackageCommand(Repository repository, String[] parts) throws InvalidCommandException {
        if (parts.length != 12) {
            throw new InvalidCommandException(ERR_PACKAGE_CMD_WRONG_ELEMENTS);
        }

        int x = 0;
        int y = 0;
        int translatedLength = 0;
        int translatedWidth = 0;
        int translatedHeight = 0;
        int measuredLength = 0;
        int measuredWidth = 0;
        int measuredHeight = 0;
        int colourCode = 0;
        boolean isFragile = false;
        int layer = 0;
        int binId = 0;

        try {
            x = Integer.parseInt(parts[1]);
            y = Integer.parseInt(parts[2]);

            translatedLength = Integer.parseInt(parts[3]);
            translatedWidth = Integer.parseInt(parts[4]);
            translatedHeight = 1;

            measuredLength = Integer.parseInt(parts[5]);
            measuredWidth = Integer.parseInt(parts[6]);
            measuredHeight = Integer.parseInt(parts[7]);

            colourCode = Integer.parseInt(parts[8]);
            isFragile = "1".equals(parts[9]);

            layer = Integer.parseInt(parts[10]);
            binId = Integer.parseInt(parts[11]);
        } catch (NumberFormatException e) {
            throw new InvalidCommandException(ERR_PACKAGE_CMD_WRONG_FORMAT);
        }

        if (!repository.binExists(binId)) {
            throw new InvalidCommandException(ERR_UNKNOWN_BIN + binId);
        }

        if (!repository.colourExists(colourCode)) {
            throw new InvalidCommandException(ERR_UNKNOWN_COLOUR + colourCode);
        }

        repository.packPackage(x, y,
                translatedLength, translatedWidth, translatedHeight,
                measuredLength, measuredWidth, measuredHeight,
                colourCode, isFragile, layer, binId);
    }

    private void parseBinCommand(Repository repository, String[] parts) throws InvalidCommandException {
        if (parts.length != 6) {
            throw new InvalidCommandException(ERR_BIN_CMD_WRONG_ELEMENTS);
        }

        int length = 0;
        int width = 0;
        int numberOfLayers = 0;
        int id = 0;

        try {
            length = Integer.parseInt(parts[1]);
            width = Integer.parseInt(parts[2]);
            numberOfLayers = Integer.parseInt(parts[3]);
            id = Integer.parseInt(parts[4]);
        } catch (NumberFormatException e) {
            throw new InvalidCommandException(ERR_BIN_CMD_WRONG_FORMAT);
        }

        String destination = parts[5];
        if (destination != null && destination.length() > 0) {
            destination = destination.replace(',', ' ');
        }

        repository.createBin(id, length, width, numberOfLayers, destination);
    }
}
