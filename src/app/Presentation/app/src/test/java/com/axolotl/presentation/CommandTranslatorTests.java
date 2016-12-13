package com.axolotl.presentation;

import com.axolotl.presentation.communication.CommandTranslator;
import com.axolotl.presentation.communication.InvalidCommandException;
import com.axolotl.presentation.model.Repository;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.mockito.Mockito;

import static org.junit.Assert.fail;
import static org.mockito.Mockito.*;


import static org.junit.Assert.assertEquals;

public class CommandTranslatorTests {
    private static String VALID_COMMAND = "B: 10 10 10 1 Local";

    @Rule
    public ExpectedException thrown = ExpectedException.none();
    private Repository repository = Mockito.mock(Repository.class);
    private CommandTranslator ct = new CommandTranslator();

    @Test
    public void translate_shouldNotAcceptNullRepository() throws Exception {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage(CommandTranslator.ERR_REPOSITORY_IS_NULL);

        ct.translate(VALID_COMMAND, null);
    }

    @Test
    public void translate_shouldNotAcceptNullCommand() throws Exception {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage(CommandTranslator.ERR_COMMAND_NULL_OR_EMPTY);

        ct.translate(null, repository);
    }

    @Test
    public void translate_shouldNotAcceptEmptyCommand() throws Exception {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage(CommandTranslator.ERR_COMMAND_NULL_OR_EMPTY);

        ct.translate("", repository);
    }

    @Test
    public void translate_shouldNotAcceptUnknownCommands() throws Exception {
        final String invalidCommands[] = {"B", "P", "A", "Invalid command",
                "Unless required by applicable law or agreed to in writing, software"};

        for (int i = 0; i < invalidCommands.length; i++) {
            try {
                ct.translate(invalidCommands[i], repository);
                fail("Invalid command was translated.");
            } catch (InvalidCommandException e) {
                if (e.getMessage() != CommandTranslator.ERR_COMMAND_UNKNOWN) {
                    fail("Wrong exception message.");
                }
            }
        }
    }
}