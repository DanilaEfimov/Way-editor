
from the developers:

about colors:
in first version of Way Editor was made such dark theme
by my friend from WarerCode - Evgeniy. It was beauty, but
in second version I want to make radioButton-triggers for
errors and warnings. And I can't reset button's text color yet.
I have decided make BLACK_WINDOW_THEME more light.
You can try old version:
#define BLACK_WINDOW_THEME {9, 27, 56}
(defines at General.h header)

about data:
why I store graph in vector-matrix? When this project was restarted
(actually it was restarted four times), one of my friends and
WarerCode programmer too - Dmitry Kiselev made one of university's
works, where he had to make C++ matrix class. Then he imagine this
thing with vector-matrix. We was searching positive sides of this
and downsides. We think that it's just a little faster.
So my contrybution was the idea of bits-menange. And in combo of
these ideas we have more than ~x16 faster store/read operations.
[x2 due to one-parameter indexation and plus x8 due to bit manange
instead bytes.]
