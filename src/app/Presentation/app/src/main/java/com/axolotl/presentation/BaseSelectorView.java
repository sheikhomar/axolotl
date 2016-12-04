package com.axolotl.presentation;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.axolotl.presentation.model.Bin;

public abstract class BaseSelectorView extends RelativeLayout {

    protected LinearLayout container;
    protected Drawable normalIcon;
    protected Drawable selectedIcon;

    public BaseSelectorView(Context context) {
        super(context);
        internalInit(null, 0);
    }

    public BaseSelectorView(Context context, AttributeSet attrs) {
        super(context, attrs);
        internalInit(attrs, 0);
    }

    public BaseSelectorView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        internalInit(attrs, defStyle);
    }

    protected void init(AttributeSet attrs, int defStyle) {

    }

    protected abstract int getNormalIconResource();
    protected abstract int getSelectedIconResource();
    protected abstract int getResourceView();
    protected abstract void onIconClick(ImageView view);

    private void internalInit(AttributeSet attrs, int defStyle) {
        inflate(getContext(), getResourceView(), this);

        this.container = (LinearLayout)findViewById(R.id.selector_container);
        this.normalIcon = ContextCompat.getDrawable(getContext(), getNormalIconResource());
        this.selectedIcon = ContextCompat.getDrawable(getContext(), getSelectedIconResource());

        init(attrs, defStyle);
    }

    protected void showIcons(int numberOfIcons) {
        createIcons(numberOfIcons);
        hideExtraIcons(numberOfIcons);
    }

    protected void clearSelection() {
        int childCount = this.container.getChildCount();
        for (int i = 0; i < childCount; i++) {
            ImageView iv = (ImageView)this.container.getChildAt(i);
            iv.setImageDrawable(this.normalIcon);
        }
    }

    private void createIcons(int numberOfVisibleImages) {
        int childCount = this.container.getChildCount();
        if (childCount < numberOfVisibleImages) {
            for (int i = childCount; i < numberOfVisibleImages; i++) {
                createNewIcon();
            }
        }
    }

    private void hideExtraIcons(int numberOfVisibleImages) {
        int childCount = this.container.getChildCount();
        for (int i = 0; i < childCount; i++) {
            if (i >= numberOfVisibleImages) {
                View view = this.container.getChildAt(i);
                view.setVisibility(GONE);
            }
        }
    }

    private void createNewIcon() {
        ImageView image = new ImageView(getContext());
        image.setImageDrawable(this.normalIcon);

        LinearLayout.LayoutParams vp = new LinearLayout.LayoutParams(
                LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        image.setLayoutParams(vp);
        image.setMaxHeight(50);
        image.setMaxWidth(50);
        image.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                clearSelection();
                ImageView imageView = (ImageView)v;
                imageView.setImageDrawable(selectedIcon);
                onIconClick(imageView);
            }
        });

        this.container.addView(image);
    }
}
