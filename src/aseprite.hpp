#pragma once
#include "../include/cute_aseprite.h"
#include "../include/raylib.h"

namespace Aseprite {
    struct Aseprite {
        ase_t* ase;         // Pointer to the cute_aseprite data.
    };

    /**
    * Tag information from an Aseprite object.
    *
    * @see LoadAsepriteTag()
    * @see LoadAsepriteTagFromIndex()
    */
    struct AsepriteTag {
        char* name;         // The name of the tag.
        int currentFrame;   // The frame that the tag is currently on
        int tagId;
        float timer;        // The countdown timer in seconds
        int direction;      // Whether we are moving forwards, or backwards through the frames
        float speed;        // The animation speed factor (1 is normal speed, 2 is double speed)
        Color color;        // The color provided for the tag
        bool loop;          // Whether to continue to play the animation when the animation finishes
        bool paused;        // Set to true to not progression of the animation
        Aseprite aseprite;  // The loaded Aseprite file
        ase_tag_t* tag;     // The active tag to act upon
    };

    /**
    * Slice data for the Aseprite.
    *
    * @see LoadAsepriteSlice()
    * @see https://www.aseprite.org/docs/slices/
    */
    struct AsepriteSlice {
        char* name;         // The name of the slice.
        Rectangle bounds;   // The rectangle outer bounds for the slice.
    };
    
    Aseprite LoadAseprite(const char* fileName);                        // Load an .aseprite file
    Aseprite LoadAsepriteFromMemory(unsigned char* fileData, unsigned int size);  // Load an aseprite file from memory
    bool IsAsepriteReady(Aseprite aseprite);                            // Check if the given Aseprite was loaded successfully
    void UnloadAseprite(Aseprite aseprite);                             // Unloads the aseprite file
    void TraceAseprite(Aseprite aseprite);                              // Display all information associated with the aseprite
    Texture GetAsepriteTexture(Aseprite aseprite);                      // Retrieve the raylib texture associated with the aseprite
    int GetAsepriteWidth(Aseprite aseprite);                            // Get the width of the sprite
    int GetAsepriteHeight(Aseprite aseprite);                           // Get the height of the sprite
    void DrawAseprite(Aseprite aseprite, int frame, int posX, int posY, Color tint);
    void DrawAsepriteV(Aseprite aseprite, int frame, Vector2 position, Color tint);
    void DrawAsepriteEx(Aseprite aseprite, int frame, Vector2 position, float rotation, float scale, Color tint);
    void DrawAsepritePro(Aseprite aseprite, int frame, Rectangle dest, Vector2 origin, float rotation, Color tint);

    // Aseprite Tag functions
    AsepriteTag LoadAsepriteTag(Aseprite aseprite, const char* name);   // Load an Aseprite tag animation sequence
    AsepriteTag LoadAsepriteTagFromIndex(Aseprite aseprite, int index); // Load an Aseprite tag animation sequence from its index
    int GetAsepriteTagCount(Aseprite aseprite);                         // Get the total amount of available tags
    bool IsAsepriteTagReady(AsepriteTag tag);                           // Check if the given Aseprite tag was loaded successfully
    void UpdateAsepriteTag(AsepriteTag* tag);                           // Update the tag animation frame
    AsepriteTag GenAsepriteTagDefault();                                // Generate an empty Tag with sane defaults
    void DrawAsepriteTag(AsepriteTag tag, int posX, int posY, Color tint);
    void DrawAsepriteTagV(AsepriteTag tag, Vector2 position, Color tint);
    void DrawAsepriteTagEx(AsepriteTag tag, Vector2 position, float rotation, float scale, Color tint);
    void DrawAsepriteTagPro(AsepriteTag tag, Rectangle dest, Vector2 origin, float rotation, Color tint);
    void SetAsepriteTagFrame(AsepriteTag* tag, int frameNumber);                           // Sets which frame the tag is currently displaying.
    int GetAsepriteTagFrame(AsepriteTag tag);

    // Aseprite Slice functions
    AsepriteSlice LoadAsepriteSlice(Aseprite aseprite, const char* name);   // Load a slice from an Aseprite based on its name.
    AsepriteSlice LoadAsperiteSliceFromIndex(Aseprite aseprite, int index); // Load a slice from an Aseprite based on its index.
    int GetAsepriteSliceCount(Aseprite aseprite);                       // Get the amount of slices that are defined in the Aseprite.
    bool IsAsepriteSliceReady(AsepriteSlice slice);                     // Return whether or not the given slice was found.
    AsepriteSlice GenAsepriteSliceDefault();                            // Generate empty Aseprite slice data.
}
