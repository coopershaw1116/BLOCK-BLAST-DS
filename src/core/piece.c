/*
    =====================================
    PIECE IMPLEMENTATION
    =====================================
    
    Piece library containing all available block shapes.
*/

#include "piece.h"

/*
    Available Piece Definitions
    ===========================
    Each piece is defined with its width, height, and cell layout.
*/
Piece pieceDefinitions[] =
{
    /* 1x1 Square */
    {
        1, 1,
        {
            {1}
        }
    },

    /* 2x1 Horizontal Line */
    {
        2, 1,
        {
            {1, 1}
        }
    },

    /* 1x2 Vertical Line */
    {
        1, 2,
        {
            {1},
            {1}
        }
    },

    /* 2x2 Square */
    {
        2, 2,
        {
            {1, 1},
            {1, 1}
        }
    },

    /* 3x1 Horizontal Line */
    {
        3, 1,
        {
            {1, 1, 1}
        }
    },

    /* 1x3 Vertical Line */
    {
        1, 3,
        {
            {1},
            {1},
            {1}
        }
    },

    /* 3x2 L-Shaped Piece */
    {
        3, 2,
        {
            {1, 0, 0},
            {1, 1, 1}
        }
    },

    /* 3x2 L-Shaped Piece Inverted */
    {
        3, 2,
        {
            {1, 1, 1},
            {1, 0, 0}
        }
    },

    /* 3x2 J-Shaped Piece */
    {
        3, 2,
        {
            {0, 0, 1},
            {1, 1, 1}
        }
    },

    /* 3x2 J-Shaped Piece Inverted */
    {
        3, 2,
        {
            {1, 1, 1},
            {0, 0, 1}
        }
    },

    /* Mini L Piece*/
    {
        2, 2,
        {
            {1, 0},
            {1, 1}
        }
    },

    /* Mini L Piece Inverted */
    {
        2, 2,
        {
            {1, 1},
            {1, 0}
        }
    },

    /* Mini J Piece */
    {
        2, 2,
        {
            {0, 1},
            {1, 1}
        }
    },

    /* Mini J Piece Inverted */
    {
        2, 2,
        {
            {1, 1},
            {0, 1}
        }
    },

    /* 3x3 Diagonal*/
    {
        3, 3,
        {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        }
    },

    /* Z-Shaped Piece R-L*/
    {
        3, 2,
        {
            {0, 1, 1},
            {1, 1, 0}
        }
    },

    /* Z-Shaped Piece L-R*/
    {
        3, 2,
        {
            {1, 1, 0},
            {0, 1, 1}
        }
    },

    /*Z-Shaped Piece R-L Sideways*/
    {
        2, 3,
        {
            {0, 1},
            {1, 1},
            {1, 0}
        }
    },

    /* Z-Shaped Piece L-R Sideways */
    {
        2, 3,
        {
            {1, 0},
            {1, 1},
            {0, 1}
        }
    },

    /* T-Shaped Piece */
    {
        3, 2,
        {
            {1, 1, 1},
            {0, 1, 0}
        }
    },

    /* T-Shaped Piece Upside-Down */
    {
        3, 2,
        {
            {0, 1, 0},
            {1, 1, 1}
        }
    },

    /*T-Shaped Piece Sideways*/
    {
        2, 3,
        {
            {0, 1},
            {1, 1},
            {0, 1}
        }
    },

    /* T-Shaped Piece Sideways Inverted */
    {
        2, 3,
        {
            {1, 0},
            {1, 1},
            {1, 0}
        }
    },

    /* 3x3 Block*/
    {
        3, 3,
        {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
        }
    },

    /* 4x1 Horizontal Line */
    {
        4, 1,
        {
            {1, 1, 1, 1}
        }
    },

    /* 1x4 Vertical Line */
    {
        1, 4,
        {
            {1},
            {1},
            {1},
            {1}
        }
    },

    /*5x1 Horizontal Line */
    {
        5, 1,
        {
            {1, 1, 1, 1, 1}
        }
    }
};

/*
    Piece Count
    ===========
    Automatically calculated from the array size.
*/
int pieceDefinitionCount =
    sizeof(pieceDefinitions) / sizeof(Piece);
