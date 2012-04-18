#import <Foundation/NSObject.h>

@interface Volume:NSObject
{
	int val;
	int min, max, step;
}

- (id)initWidthMin:(int)a max:(int)b step:(int)s;
- (int)value;
- (id)up;
- (id)down;
@end

