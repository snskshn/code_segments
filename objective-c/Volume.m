#import "Volume.h"

@implementation Volume
- (id)initWidthMin:(int)a max:(int)b step:(int)s
{
	self = [super init];
	if(self!=nil){
		val = min = a;
		max = b;
		step = s;
	}
	return self;
}

- (int)value
{
	return val;
}

- (id)up
{
	if((val+=step) > max)
		val = max;
	return self;
}

- (id)down
{
	if((val-=step)<min)
		val = min;
	return self;
}
@end

