package com.axolotl.presentation;

import com.axolotl.presentation.communication.CommandParser;
import com.axolotl.presentation.communication.InvalidCommandException;
import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.ColourCodeParser;
import com.axolotl.presentation.model.Repository;

import org.junit.Assert;
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

public class RepositoryTests {

    // Mockito tutorial: http://www.vogella.com/tutorials/Mockito/article.html

    // Tells Mockito to mock this instance
    @Mock private ColourCodeParser colourCodeParser;

    // Tells Mockito to create the mocks based on the @Mock annotation
    @Rule public MockitoRule mockitoRule = MockitoJUnit.rule();

    // For testing exceptions
    @Rule public ExpectedException thrown = ExpectedException.none();

    @Test
    public void createBin_shouldCreateCorrectBinObject() throws Exception {
        Repository repository = new Repository(colourCodeParser);

        Assert.assertNotNull(repository.getBins().size());
        Assert.assertEquals(0, repository.getBins().size());

        repository.createBin(1, 2, 3, 4, "Local");

        Assert.assertEquals(1, repository.getBins().size());
        Bin bin = repository.getBins().get(0);

        Assert.assertEquals(1, bin.getId());
        Assert.assertEquals(2, bin.getLength());
        Assert.assertEquals(3, bin.getWidth());
        Assert.assertEquals(4, bin.getNumberOfLayers());
        Assert.assertEquals("Local", bin.getDestination());
    }

    @Test
    public void getBinById_shouldReturnCorrectBin() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(1, 2, 3, 4, "France");
        repository.createBin(10, 11, 12, 13, "Denmark");

        Bin bin1 = repository.getBinById(1);
        Bin bin10 = repository.getBinById(10);

        Assert.assertNotNull(bin1);
        Assert.assertNotNull(bin10);

        Assert.assertEquals(1, bin1.getId());
        Assert.assertEquals(2, bin1.getLength());
        Assert.assertEquals(3, bin1.getWidth());
        Assert.assertEquals(4, bin1.getNumberOfLayers());
        Assert.assertEquals("France", bin1.getDestination());

        Assert.assertEquals(10, bin10.getId());
        Assert.assertEquals(11, bin10.getLength());
        Assert.assertEquals(12, bin10.getWidth());
        Assert.assertEquals(13, bin10.getNumberOfLayers());
        Assert.assertEquals("Denmark", bin10.getDestination());
    }

    @Test
    public void getBinById_shouldReturnNullIfBinIdIsNotFound() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(1, 2, 3, 4, "Local");
        repository.createBin(10, 11, 12, 13, "Local");

        Assert.assertNull(repository.getBinById(0));
        Assert.assertNull(repository.getBinById(2));
        Assert.assertNull(repository.getBinById(11));
        Assert.assertNull(repository.getBinById(9));
    }


    @Test
    public void binExists_shouldReturnCorrectValue() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(1000, 2, 3, 4, "France");
        repository.createBin(2000, 5, 12, 13, "Denmark");

        Assert.assertTrue(repository.binExists(1000));
        Assert.assertTrue(repository.binExists(2000));
        Assert.assertFalse(repository.binExists(1));
        Assert.assertFalse(repository.binExists(2));
        Assert.assertFalse(repository.binExists(10));
    }

    @Test
    public void getSelectedPackage_shouldReturnCorrectValue() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(1000, 2, 3, 4, "France");
        repository.createBin(2000, 5, 12, 13, "Denmark");

        Assert.assertTrue(repository.binExists(1000));
        Assert.assertTrue(repository.binExists(2000));
        Assert.assertFalse(repository.binExists(1));
        Assert.assertFalse(repository.binExists(2));
        Assert.assertFalse(repository.binExists(10));
    }
}