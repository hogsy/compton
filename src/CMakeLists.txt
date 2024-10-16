add_executable(chorizo WIN32
        entity/entity.cpp
        entity/entity_manager.cpp

        GUI/GUIButton.cpp
        GUI/GUICursor.cpp
        GUI/GUIPanel.cpp
        GUI/GUIPieMenu.cpp
        GUI/GUIStyleSheet.cpp

        input/input_action.cpp
        input/input_manager.cpp

        render/render.cpp
        render/render_bitmap_font.cpp
        render/render_sprite_sheet.cpp

        app.cpp
        console.cpp
        filesystem.cpp

        background.cpp
        game_mode.cpp

        image_manager.cpp
        random.cpp
        script.cpp
        script_parser.cpp
        serializer.cpp
)

set_target_properties(chorizo PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

if (WIN32)
    target_include_directories(chorizo PRIVATE src/3rdparty/allegro/include/)
    target_link_directories(chorizo PRIVATE src/3rdparty/allegro/lib/)
    target_link_libraries(chorizo -Wl,-Bstatic stdc++ winpthread -Wl,-Bdynamic -static-libstdc++ -static-libgcc)
endif (WIN32)
target_link_libraries(chorizo
        lisp
        plcore

        allegro
        allegro_dialog
        allegro_audio
        allegro_acodec
)
