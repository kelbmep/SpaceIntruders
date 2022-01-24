package com.grudinskiy.androidlauncher;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity {
    @Override
    final protected String[] getLibraries() {
        return new String[]{"SpaceIntruders"};
    }

    @Override
    final protected String getMainFunction() {
        return "main";
    }
}