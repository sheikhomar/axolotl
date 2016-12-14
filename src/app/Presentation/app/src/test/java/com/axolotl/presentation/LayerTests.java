package com.axolotl.presentation;

import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.PackageDimension;
import com.axolotl.presentation.model.PackedPackage;

import junit.framework.Assert;

import org.junit.Rule;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

import static org.junit.Assert.fail;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class LayerTests {

    // Tells Mockito to mock this instance
    @Mock private Package aPackage;

    @Rule public MockitoRule mockitoRule = MockitoJUnit.rule();

    @Test(expected = IllegalArgumentException.class)
    public void constructor_shouldNotAllowZeroLength() throws Exception {
        new Layer(0, 4);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructor_shouldNotAllowZeroWidth() throws Exception {
        new Layer(6, 0);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructor_shouldNotAllowNegativeWidth() throws Exception {
        new Layer(8, -1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructor_shouldNotAllowNegativeLength() throws Exception {
        new Layer(-10, 15);
    }

    @Test
    public void constructor_shouldSetFields() throws Exception {
        Layer layer = new Layer(42, 43);
        Assert.assertEquals(42, layer.getLength());
        Assert.assertEquals(43, layer.getWidth());
    }

    @Test
    public void pack_shouldCreatePackedPackageCorrectly() throws Exception {
        Layer layer = new Layer(15, 15);
        layer.pack(aPackage, 1, 2);

        Assert.assertEquals(layer.getNumberOfPackages(), 1);

        PackedPackage pp = layer.get(0);
        Assert.assertSame(aPackage, pp.getPackage());
        Assert.assertEquals(1, pp.getX());
        Assert.assertEquals(2, pp.getY());
    }


    @Test
    public void pack_shouldPackMultiplePackagesCorrectly() throws Exception {
        Layer layer = new Layer(15, 15);
        layer.pack(aPackage, 5, 10);

        Package p2 = Mockito.mock(Package.class);

        layer.pack(p2, 3, 4);

        Assert.assertEquals(layer.getNumberOfPackages(), 2);

        PackedPackage pp1 = layer.get(0);
        Assert.assertSame(aPackage, pp1.getPackage());
        Assert.assertEquals(5, pp1.getX());
        Assert.assertEquals(10, pp1.getY());

        PackedPackage pp2 = layer.get(1);
        Assert.assertSame(p2, pp2.getPackage());
        Assert.assertEquals(3, pp2.getX());
        Assert.assertEquals(4, pp2.getY());
    }

    @Test
    public void select_shouldReturnNullWhenThereAresNoPackages() throws Exception {
        Layer layer = new Layer(10, 10);

        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                Assert.assertNull(layer.select(i, j));
            }
        }
    }
    @Test
    public void select_shouldReturnNullIfPositionIsOutside() throws Exception {
        Layer layer = new Layer(5, 10);

        final int PACKAGE_LENGTH = 2;
        final int PACKAGE_WIDTH = 4;
        final int POS_X = 2;
        final int POS_Y = 3;

        PackageDimension d = new PackageDimension(PACKAGE_LENGTH, PACKAGE_WIDTH, 1);
        when(aPackage.getDimension()).thenReturn(d);
        layer.pack(aPackage, POS_X, POS_Y);

        // Verify lower bounds
        for (int x = -2; x < POS_X; x++) {
            for (int y = -3; y < POS_Y; y++) {
                Assert.assertNull(layer.select(x, y));
            }
        }

        // Verify upper bounds
        for (int x = POS_X + PACKAGE_LENGTH; x < 10; x++) {
            for (int y = POS_Y + PACKAGE_WIDTH; y < 10; y++) {
                Assert.assertNull(layer.select(x, y));
            }
        }
    }

    @Test
    public void select_shouldReturnCorrectPackage() throws Exception {
        Layer layer = new Layer(5, 10);

        final int PACKAGE_LENGTH = 2;
        final int PACKAGE_WIDTH = 4;

        PackageDimension d = new PackageDimension(PACKAGE_LENGTH, PACKAGE_WIDTH, 1);
        when(aPackage.getDimension()).thenReturn(d);
        layer.pack(aPackage, 2, 2);


        for (int x = 2; x <= PACKAGE_LENGTH; x++) {
            for (int y = 2; y <= PACKAGE_WIDTH; y++) {
                Assert.assertSame(layer.select(x, y), aPackage);
            }
        }
    }

}