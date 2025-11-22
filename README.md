This code demostrated how we can use all mouse events in gtkmm4

    on_MB1_cli(); // Mouse left button clicked
    
    on_MB2_cli(); // Mouse middle button clicked
    
    on_MB3_cli(); // Mouse right button clicked
    
    on_MB8_cli(); // Mouse optional8 button clicked - only more than 3 buttons mouse have this button
    
    on_MB9_cli(); // Mouse optional9 button clicked - only more than 3 buttons mouse have this button
    

    on_MB1_double_cli(); // Mouse left button double clicked
    
    on_MB1_triple_cli(); // Mouse left button triple clicked
    
    on_MB1_quad_cli(); // Mouse left button quad clicked
    
    on_MB2_double_cli(); // Mouse middle button double clicked
    
    on_MB3_double_cli(); // Mouse right button double clicked
    
    on_MB8_double_cli(); // Mouse optional8 button double clicked
    
    on_MB9_double_cli(); // Mouse optional9 button double clicked
    

    on_MB1_rel(); // Mouse left button released
    
    on_MB2_rel(); // Mouse middle button released
    
    on_MB3_rel(); // Mouse right button released
    
    on_MB8_rel(); // Mouse optional8 button released
    
    on_MB9_rel(); // Mouse optional9 button released
    

    on_MB1_drag_start(); // Mouse left button dragged
    
    on_MB1_drag_update(double dx, double dy);
    
    on_MB1_drag_end();
    

    on_MB2_drag_start(); // Mouse middle button dragged
    
    on_MB2_drag_update(double dx, double dy);
    
    on_MB2_drag_end();
    

    on_MB3_drag_start(); // Mouse right button dragged
    
    on_MB3_drag_update(double dx, double dy);
    
    on_MB3_drag_end();
    

    on_MB8_drag_start(); // Mouse optional8 button dragged
    
    on_MB8_drag_update(double dx, double dy);
    
    on_MB8_drag_end();
    

    on_MB9_drag_start(); // Mouse optional9 button dragged
    
    on_MB9_drag_update(double dx, double dy);
    
    on_MB9_drag_end();
    

    on_MB21_drag_start(); // Mouse middle+left button dragged
    
    on_MB21_drag_update(double dx, double dy);
    
    on_MB21_drag_end();
    

    on_MB23_drag_start(); // Mouse middle+right button dragged
    
    on_MB23_drag_update(double dx, double dy);
    
    on_MB23_drag_end(); 
    

there is sample debug log:

[SYSTEM] Mouse ENTERED widget at (-2.92578, 482.227)

MB1: Click (Press)

MB1: Release

MB1: Click (Press)

MB1: Release

MB1: Click (Press)

MB1: Release

MB1: Click (Press)

MB1: Release

MB9: Click (Press)

[SYSTEM] Raw Release Event caught for MB9

MB9: Release

MB9: Click (Press)

[SYSTEM] Raw Release Event caught for MB9

MB9: Release

MB9: Click (Press)

[SYSTEM] Raw Release Event caught for MB9

MB9: Release

MB9: Release

MB9: DOUBLE Click!

MB9: Click (Press)

MB9: Drag START

MB9: Drag UPDATE -1.64844,6.60156

MB9: Drag UPDATE -1.64844,8.25391

MB9: Drag UPDATE 0,9.90625

MB9: Drag UPDATE 72.6484,90.8086

[SYSTEM] Raw Release Event caught for MB9

MB9: Release

MB9: Drag END

MB9: Click (Press)

MB9: Drag START

MB9: Drag UPDATE 8.25391,13.207

MB9: Drag UPDATE 11.5547,14.8594

MB9: Drag UPDATE 14.8594,16.5078

MB9: Drag UPDATE 168.406,69.3438

[SYSTEM] Raw Release Event caught for MB9

MB9: Release

MB9: Drag END

MB8: Click (Press)

MB8: Drag START

MB8: Drag UPDATE 8.25391,-1.64844

MB8: Drag UPDATE 9.90625,-1.64844

MB8: Drag UPDATE 13.207,-1.64844

MB8: Drag UPDATE 155.199,31.3711

[SYSTEM] Raw Release Event caught for MB8

MB8: Release

MB8: Drag END

MB2: Click (Press)

MB2: Drag START

MB2: Drag UPDATE 3.30078,6.60547

MB2: Drag UPDATE 3.30078,8.25391

MB2: Drag UPDATE 4.95312,9.90625

MB2: Drag UPDATE 14.8594,26.418

MB2: Release

MB2: Drag END

MB2: Click (Press)

MB2: Drag START

MB2: Drag UPDATE 3.30078,9.90625

MB2: Drag UPDATE 4.95312,14.8594

MB2: Drag UPDATE 6.60156,18.1602

MB2: Drag UPDATE 42.9258,82.5508

MB2: Release

MB2: Drag END

MB2: Click (Press)

MB2: Drag START

MB2: Drag UPDATE 3.30078,9.90625

MB2: Drag UPDATE 3.30078,11.5547

MB2: Drag UPDATE 3.30078,14.8594

MB2: Drag UPDATE 6.60547,36.3242

MB2: Release

MB2: Drag END

MB3: Click (Press)

MB3: Drag START

MB3: Drag UPDATE 0,8.25391

MB3: Drag UPDATE 0,9.90625

MB3: Drag UPDATE 0,13.207

MB3: Drag UPDATE -1.65234,18.1641

MB3: Release

MB3: Drag END

MB2: Click (Press)

MB2: Drag START

MB2: Drag UPDATE 1.64844,8.25391

MB2: Drag UPDATE 1.64844,9.90625

MB2: Drag UPDATE 1.64844,11.5547

MB2: Drag UPDATE 3.30078,34.6719

MB3: Click (Press)

MB2+3: Combo Drag UPDATE 3.30078,36.3242

MB2+3: Combo Drag UPDATE -13.2109,107.316

MB2+3: Combo Drag UPDATE -14.8594,107.316

MB3: Release

[SYSTEM] FORCE END: Secondary combo button released

MB2: Drag END

MB2: Click (Press)

MB2: Drag START

MB2: Drag UPDATE -1.64844,8.25391

MB2: Drag UPDATE -1.64844,9.90625

MB1: Click (Press)

MB2+1: Combo Drag UPDATE -1.64844,11.5586

MB2+1: Combo Drag UPDATE -1.64844,13.207

MB2+1: Combo Drag UPDATE -1.64844,16.5117

MB2+1: Combo Drag UPDATE -4.95312,97.4102

[SYSTEM] FORCE END: Safety Check Fail

MB2: Drag END

Scroll: 0, -1

Scroll: 0, -1

Scroll: 0, 1

Scroll: 0, 1

MB1: Click (Press)

MB1: Release

MB1: DOUBLE Click!

MB1: Click (Press)

MB1: Release

MB1: DOUBLE Click!

MB1: TRIPLE Click!

MB1: Click (Press)

MB1: Release

MB1: DOUBLE Click!

MB1: TRIPLE Click!

MB1: Click (Press)

MB1: Release

MB1: DOUBLE Click!

MB1: TRIPLE Click!

MB1: QUAD Click! (4x)

MB2: Click (Press)

MB2: Release

MB2: Click (Press)

MB2: Release

MB2: DOUBLE Click!

MB3: Click (Press)

MB3: Release

MB3: DOUBLE Click!

MB1: Click (Press)

MB1: Release

MB1: DOUBLE Click!

MB9: Click (Press)

[SYSTEM] Raw Release Event caught for MB9

MB9: Release

MB9: Release

MB9: DOUBLE Click!

MB8: Click (Press)

MB8: Drag START

MB8: Drag UPDATE -6.60547,-3.30078

MB8: Drag UPDATE -6.60547,-4.95312

MB8: Drag UPDATE -9.90625,-13.207

[SYSTEM] Raw Release Event caught for MB8

MB8: Release

MB8: Drag END

MB8: Click (Press)

[SYSTEM] Raw Release Event caught for MB8

MB8: Release

MB8: Release

MB8: DOUBLE Click!

MB1: Click (Press)

MB1: Release
