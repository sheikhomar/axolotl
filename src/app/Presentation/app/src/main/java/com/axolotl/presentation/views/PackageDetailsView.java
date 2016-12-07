package com.axolotl.presentation.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.axolotl.presentation.R;
import com.axolotl.presentation.model.Package;

public class PackageDetailsView extends RelativeLayout {
    private TextView destinationView;
    private TextView dimensionView;
    private TextView colourView;

    public PackageDetailsView(Context context) {
        super(context);
        init(null, 0);
    }

    public PackageDetailsView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public PackageDetailsView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
    }

    public void setPackage(Package aPackage) {
        if (aPackage != null) {
            dimensionView.setText(aPackage.getRealDimension().toString() + " mm");
            colourView.setText(aPackage.getColour().getName());
            destinationView.setText("");
        } else {
            dimensionView.setText("");
            colourView.setText("");
            destinationView.setText("");
        }
    }

    private void init(AttributeSet attrs, int defStyle) {
        inflate(getContext(), R.layout.package_details_view, this);

        this.destinationView = (TextView)findViewById(R.id.package_details_destination);
        this.dimensionView = (TextView)findViewById(R.id.package_details_dimension);
        this.colourView = (TextView)findViewById(R.id.package_details_colour);
    }
}
