# This repository only contains the source code and config files
Game assets, plugins directory, binaries etc. are not part of the repository.

## unresolved bugs

-   A previously replaced ally shows up in the UI overlay as if it was their turn. When skill is chosen, another ally that 
    is currently in the party performs it. This apparently doesn't happen on new game or after loading from saves, 
    only when the player replaces an ally from the menu or the new ally joins the party after certain cutscenes.

-   A turn starts for two allies at once. One can heal another all, while another one attacks that same ally 
    or they use the same item simultaneously. Sometimes one character shows in the overlay and another one performs their skill.
    This seems to also only happen after an ally has been replaced, but I cannot state it with certainty.

-   A game over popup window shows up after one ally dies instead of the entire party. It also seems to be an ally that replaced a previous one,
    but this also cannot be stated with 100% certainty.
