package com.axolotl.presentation;

import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.ColourCodeParser;
import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.Repository;

import org.junit.Assert;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNotSame;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.CALLS_REAL_METHODS;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
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
    public void packPackage_shouldAutomaticallySelectLastestPackage() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(5, 2, 3, 4, "France");

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 5);
        Package selectedPackage1 = repository.getSelectedPackage();
        assertNotNull(selectedPackage1);

        repository.packPackage(50, 51, 52, 53, 54, 55, 56, 57, 0, false, 3, 5);
        Package selectedPackage2 = repository.getSelectedPackage();
        assertNotNull(selectedPackage2);
        assertNotSame(selectedPackage1, selectedPackage2);
    }

    @Test
    public void packPackage_shouldSelectTheLastestBin() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(5, 2, 3, 4, "France");
        repository.createBin(10, 20, 30, 3, "Denmark");

        Bin binToFrance = repository.getBinById(5);
        Bin binToDenmark = repository.getBinById(10);

        // Package to France
        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 5);
        assertSame(binToFrance, repository.getSelectedBin());

        // Package to Denmark
        repository.packPackage(50, 51, 52, 53, 54, 55, 56, 57, 0, false, 3, 10);
        assertSame(binToDenmark, repository.getSelectedBin());

        // Package to France
        repository.packPackage(6, 34, 1, 3, 4, 45, 41, 124, 1, true, 3, 5);
        assertSame(binToFrance, repository.getSelectedBin());
    }

    @Test
    public void packPackage_shouldSelectLayerOfTheLastestPackage() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(12, 20, 30, 3, "Sweden");
        Bin binToSweden = repository.getBinById(12);
        Layer layer1 = binToSweden.getLayerAt(0);
        Layer layer2 = binToSweden.getLayerAt(1);
        Layer layer3 = binToSweden.getLayerAt(2);

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 12);
        assertSame(layer3, repository.getSelectedLayer());

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 1, 12);
        assertSame(layer1, repository.getSelectedLayer());

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 2, 12);
        assertSame(layer2, repository.getSelectedLayer());
    }

    @Test
    public void packPackage_shouldNotChangeSelectionOnManualPackageSelection() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(16, 42, 43, 42, "Netherlands");

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 16);
        Package selectedPackage = repository.getSelectedPackage();

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 16);

        // Manually select a package
        repository.togglePackageSelection(selectedPackage);

        repository.packPackage(4, 1, 2, 3, 4, 5, 234, 34, 0, false, 3, 16);
        assertSame(selectedPackage, repository.getSelectedPackage());

        // Deselect package
        repository.togglePackageSelection(selectedPackage);
        assertNull(repository.getSelectedPackage());

        repository.packPackage(7, 5, 6, 14, 46, 8, 4, 4, 0, true, 3, 16);
        assertNotSame(selectedPackage, repository.getSelectedPackage());
    }


    @Test
    public void selectBin_shouldRemoveDefaultPackageSelectionIfBinSelectionChanges() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(17, 20, 30, 3, "Italy");
        repository.createBin(29, 20, 30, 3, "Switzerland");

        Bin binToItaly = repository.getBinById(17);
        Bin binToSwitzerland = repository.getBinById(29);

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 17);
        repository.packPackage(14, 1, 1, 5, 5, 6, 34, 4, 0, false, 3, 29);
        assertNotNull(repository.getSelectedPackage());
        assertSame(binToSwitzerland, repository.getSelectedBin());

        repository.selectBin(binToItaly);
        assertNull(repository.getSelectedPackage());
    }

    @Test
    public void selectBin_shouldChangePackageAndLayerSelectionsIfBinSelectionDoesNotChange() throws Exception {
        Repository repository = new Repository(colourCodeParser);
        repository.createBin(17, 20, 30, 3, "Italy");
        repository.createBin(29, 20, 30, 3, "Switzerland");

        Bin binToItaly = repository.getBinById(17);
        Bin binToSwitzerland = repository.getBinById(29);

        repository.packPackage(10, 11, 12, 13, 14, 45, 41, 124, 0, true, 3, 17);
        repository.packPackage(14, 1, 1, 5, 5, 6, 34, 4, 0, false, 3, 29);
        Package selectedPackage = repository.getSelectedPackage();
        assertNotNull(selectedPackage);
        assertSame(binToSwitzerland, repository.getSelectedBin());

        repository.selectBin(binToSwitzerland);
        assertSame(selectedPackage, repository.getSelectedPackage());
    }
}