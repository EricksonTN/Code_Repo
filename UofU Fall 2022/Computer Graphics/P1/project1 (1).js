// bgImg is the background image to be modified.
// fgImg is the foreground image.
// fgOpac is the opacity of the foreground image.
// fgPos is the position of the foreground image in pixels. It can be negative and (0,0) means the top-left pixels of the foreground and background are aligned.
function composite( bgImg, fgImg, fgOpac, fgPos )
{
    
    for (var i = 0; i < bgImg.data.length; i+=4)
    {
        var currentX = (i/4) % bgImg.width;
        var currentY = Math.floor((i/4) / bgImg.width);
        
        var overlapX = currentX - fgPos.x;
        var overlapY = currentY - fgPos.y;

        if ((overlapX >= 0) && (overlapX < fgImg.width) && (overlapY >= 0) && (overlapY < fgImg.height))
        {
            var fgTempPos = ((overlapY * fgImg.width) + overlapX) * 4;           

            if (fgImg.data[fgTempPos + 3] == 0)
            {
                // Red Channel
                bgImg.data[i] = bgImg.data[i];

                // Green Channel
                bgImg.data[i+1] = bgImg.data[i+1];

                // Blue Channel
                bgImg.data[i+2] = bgImg.data[i+2];
            }

            else {
                // Red Channel
                bgImg.data[i] = ((fgOpac*fgImg.data[fgTempPos])+((1-fgOpac)*bgImg.data[i]));

                // Green Channel
                bgImg.data[i+1] = ((fgOpac*fgImg.data[fgTempPos+1])+((1-fgOpac)*bgImg.data[i+1]));

                // Blue Channel
                bgImg.data[i+2] = ((fgOpac*fgImg.data[fgTempPos+2])+((1-fgOpac)*bgImg.data[i+2]));
            }
        }

    }
}
