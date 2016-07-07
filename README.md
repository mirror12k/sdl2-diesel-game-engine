# sdl2-diesel-game-engine
a bare-bones 2d game engine written in cpp with SDL2

## description
a slight departure from my previous engine designs intended to support dual-threaded execution.
- entities requiring sprites only reference them by name
- the drawing_context detects a missing SDL_Texture of a sprite and retrieves it JIT by filename (loading it in if necessary)
- entities have no parent, as they always belong directly to the update_context
- removed entities are immediately destroyed to prevent any technicalities with re-adding entities
- a dynamic loader is implemented which allows python-style instantiation of entities from text files
- services allow specialized entities to be created and retrieved by name from the update_context

## TODO:
- actually multi-thread it
- references in dynamic loading
- camera support
- dynamic frame delay
- scripting language?
