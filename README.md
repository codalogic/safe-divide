safe-divide
===========

safe-divide is a simple experiment exploring the impact of code clarity
when the _calling_ function is allowed to specify the exception thrown
when an error condition is detected.  It is derived from the example
functions shown in the Expected<T,E> proposal, documented at
http://www.hyc.io/boost/expected-proposal.pdf .

safe-divide is another attempt to combine the strengths of both exception
based processing and return code processing.

As documented in the Expected<T,E> proposal, use of exception yields a
clean main code path, but the exceptions that are thrown are often invisible,
and it's often hard to verify that the error conditions have actually been
catered for.  On the other hand, return code based coding clearly shows that
error conditions have been handled, but this is at the expense of polluting
the main code path.

Allowing the _calling_ function to specify the exception to be thrown if there
are errors, allows the calling function to change the code execution flow when
the called function encounters error conditions.  This is particularly 
significant when multiple operations of the same type are performed, but you want
to perform different error recovery / reporting based on whether the first, 
second, etc instance of the operation failed (For example, see
test_multiple_safe_divide_bad() and test_multiple_f1_ok() in the source code
file).

I believe this approach allows for some cleaner code.  There is visibility that
error conditions are being handled, but it doesn't clutter up the main code path,
thus combining the benefits of traditional exception based coding and return
code based coding.
