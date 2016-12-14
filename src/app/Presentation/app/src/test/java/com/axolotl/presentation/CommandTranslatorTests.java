package com.axolotl.presentation;

import com.axolotl.presentation.communication.CommandTranslator;
import com.axolotl.presentation.communication.InvalidCommandException;
import com.axolotl.presentation.model.Repository;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.mockito.Matchers;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

import static org.junit.Assert.fail;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

public class CommandTranslatorTests {
    private static String VALID_COMMAND = "B: 10 10 10 1 Local";
    private CommandTranslator ct = new CommandTranslator();

    // Mockito tutorial: http://www.vogella.com/tutorials/Mockito/article.html

    // Tells Mockito to mock this instance
    @Mock private Repository repository;

    // Tells Mockito to create the mocks based on the @Mock annotation
    @Rule public MockitoRule mockitoRule = MockitoJUnit.rule();

    // For testing exceptions
    @Rule public ExpectedException thrown = ExpectedException.none();


    @Test
    public void translate_shouldNotAcceptNullRepository() throws Exception {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage(CommandTranslator.ERR_REPOSITORY_IS_NULL);

        ct.translate(VALID_COMMAND, null);
        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptNullCommand() throws Exception {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage(CommandTranslator.ERR_CMD_NULL_OR_EMPTY);

        ct.translate(null, repository);
        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptEmptyCommand() throws Exception {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage(CommandTranslator.ERR_CMD_NULL_OR_EMPTY);

        ct.translate("", repository);
        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptUnknownCommands() throws Exception {
        final String invalidCommands[] = {
                "B",
                "P",
                "A",
                "Invalid command",
                "Unless required by applicable law or agreed to in writing, software"};

        for (int i = 0; i < invalidCommands.length; i++) {
            try {
                ct.translate(invalidCommands[i], repository);
                fail("Expected invalid command exception!");
            } catch (InvalidCommandException e) {
                if (e.getMessage() != CommandTranslator.ERR_CMD_UNKNOWN) {
                    fail("Wrong exception message.");
                }
            }
        }

        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptInvalidBinCommands() throws Exception {
        final String invalidCommands[] = {
                "B: ",
                "B: 1 ",
                "B: 1 2",
                "B: 1 2 3",
                "B: 1 2 3 4",
        };

        for (int i = 0; i < invalidCommands.length; i++) {
            try {
                ct.translate(invalidCommands[i], repository);
                fail("Expected invalid command exception!");
            } catch (InvalidCommandException e) {
                if (e.getMessage() != CommandTranslator.ERR_BIN_CMD_WRONG_ELEMENTS) {
                    fail("Wrong exception message.");
                }
            }
        }

        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptBinCommandAlphaNumeric() throws Exception {
        thrown.expect(InvalidCommandException.class);
        thrown.expectMessage(CommandTranslator.ERR_BIN_CMD_WRONG_FORMAT);

        ct.translate("B: C D E F G", repository);

        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldParseBinCommandCorrectly() throws Exception {
        ct.translate("B: 10 20 3 1 Isle,of,Mors", repository);

        verify(repository, times(1)).createBin(
                Matchers.eq(1),
                Matchers.eq(10),
                Matchers.eq(20),
                Matchers.eq(3),
                Matchers.eq("Isle of Mors"));
    }

    @Test
    public void translate_shouldNotAcceptPackageCommandWithWrongNumberOfParts() throws Exception {
        final String invalidCommands[] = {
                "P: ",
                "P: 1 ",
                "P: 1 2",
                "P: 1 2 3",
                "P: 1 2 3 4",
                "P: 1 2 3 4 5",
                "P: 1 2 3 4 6 7",
                "P: 1 2 3 4 6 7 8",
                "P: 1 2 3 4 6 7 8 9",
                "P: 1 2 3 4 6 7 8 9 10",
                "P: 1 2 3 4 6 7 8 9 10 11"
        };

        for (int i = 0; i < invalidCommands.length; i++) {
            try {
                ct.translate(invalidCommands[i], repository);
                fail("Expected InvalidCommandException!");
            } catch (InvalidCommandException e) {
                if (e.getMessage() != CommandTranslator.ERR_PACKAGE_CMD_WRONG_ELEMENTS) {
                    fail("Wrong exception message: " + e.getMessage());
                }
            }
        }

        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptPackageCommandAlphaNumeric() throws Exception {
        thrown.expect(InvalidCommandException.class);
        thrown.expectMessage(CommandTranslator.ERR_PACKAGE_CMD_WRONG_FORMAT);

        ct.translate("P: A B C D E F G H I J K", repository);

        verifyNoMoreInteractions(repository);
    }

    @Test
    public void translate_shouldNotAcceptPackageCommandWithNonExistentBinId() throws Exception {
        thrown.expect(InvalidCommandException.class);
        thrown.expectMessage(CommandTranslator.ERR_UNKNOWN_BIN);

        when(repository.binExists(10)).thenReturn(false);

        // x y tl tw ml mw mh colour fragile layer binId
        ct.translate("P: 0 1 2 3 4 5 6 7 8 9 10", repository);
    }

    @Test
    public void translate_shouldNotAcceptPackageCommandWithNonExistentColourCode() throws Exception {
        thrown.expect(InvalidCommandException.class);
        thrown.expectMessage(CommandTranslator.ERR_UNKNOWN_COLOUR);

        when(repository.binExists(20)).thenReturn(true);
        when(repository.colourExists(17)).thenReturn(false);

        // x y tl tw ml mw mh colour fragile layer binId
        ct.translate("P: 10 11 12 13 14 15 16 17 18 19 20", repository);
    }

    @Test
    public void translate_shouldParsePackageCommandCorrectly() throws Exception {
        when(repository.colourExists(0)).thenReturn(true);
        when(repository.binExists(21)).thenReturn(true);

        // x y tl tw ml mw mh colour fragile layer binId
        ct.translate("P: 0 2 2 6 16 32 68 0 1 2 21", repository);

        // int x, int y, int translatedLength, int translatedWidth, int translatedHeight, int measuredLength, int measuredWidth, int measuredHeight, int colourCode, boolean isFragile, int layerNo, int binId
        verify(repository, times(1)).packPackage(
                Matchers.eq(0),
                Matchers.eq(2),
                Matchers.eq(2),
                Matchers.eq(6),
                Matchers.eq(1),
                Matchers.eq(16),
                Matchers.eq(32),
                Matchers.eq(68),
                Matchers.eq(0),
                Matchers.eq(true),
                Matchers.eq(2),
                Matchers.eq(21));
    }
}