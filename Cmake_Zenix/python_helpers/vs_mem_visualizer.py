"""
The file would look like this:
		[0]	BORDER (8 '\b')	Tetris::BodyType
		[1]	BLANK (0 '\0')	Tetris::BodyType
		[2]	BLANK (0 '\0')	Tetris::BodyType
		[3]	BLANK (0 '\0')	Tetris::BodyType
		[4]	BLANK (0 '\0')	Tetris::BodyType
		[5]	BLANK (0 '\0')	Tetris::BodyType
		[6]	BLANK (0 '\0')	Tetris::BodyType
		[7]	BLANK (0 '\0')	Tetris::BodyType
		[8]	BLANK (0 '\0')	Tetris::BodyType
		[9]	BLANK (0 '\0')	Tetris::BodyType
		[10]	BLANK (0 '\0')	Tetris::BodyType
		[11]	BORDER (8 '\b')	Tetris::BodyType
		[12]	BORDER (8 '\b')	Tetris::BodyType
		[13]	BLANK (0 '\0')	Tetris::BodyType
		[14]	BLANK (0 '\0')	Tetris::BodyType
		[15]	BLANK (0 '\0')	Tetris::BodyType
		[16]	BLANK (0 '\0')	Tetris::BodyType
		[17]	BLANK (0 '\0')	Tetris::BodyType
		[18]	BLANK (0 '\0')	Tetris::BodyType
		[19]	BORDER (8 '\b')	Tetris::BodyType
		[20]	BORDER (8 '\b')	Tetris::BodyType
		[21]	BLANK (0 '\0')	Tetris::BodyType
		[22]	BLANK (0 '\0')	Tetris::BodyType
		[23]	BORDER (8 '\b')	Tetris::BodyType
		[24]	BORDER (8 '\b')	Tetris::BodyType
		[25]	BLANK (0 '\0')	Tetris::BodyType
		[26]	BLANK (0 '\0')	Tetris::BodyType
		[27]	BLANK (0 '\0')	Tetris::BodyType
		[28]	BLANK (0 '\0')	Tetris::BodyType
		[29]	BLANK (0 '\0')	Tetris::BodyType
		[30]	BLANK (0 '\0')	Tetris::BodyType
		[31]	BORDER (8 '\b')	Tetris::BodyType
		[32]	BORDER (8 '\b')	Tetris::BodyType
		[33]	BORDER (8 '\b')	Tetris::BodyType
		[34]	BORDER (8 '\b')	Tetris::BodyType
		[35]	BORDER (8 '\b')	Tetris::BodyType
		[36]	BORDER (8 '\b')	Tetris::BodyType
		[37]	BORDER (8 '\b')	Tetris::BodyType
		[38]	BORDER (8 '\b')	Tetris::BodyType
		[39]	BORDER (8 '\b')	Tetris::BodyType
		[40]	BORDER (8 '\b')	Tetris::BodyType
		[41]	BORDER (8 '\b')	Tetris::BodyType
		[42]	BORDER (8 '\b')	Tetris::BodyType
		[43]	BORDER (8 '\b')	Tetris::BodyType
		[47]	BORDER (8 '\b')	Tetris::BodyType
		[48]	BORDER (8 '\b')	Tetris::BodyType
		[49]	BLANK (0 '\0')	Tetris::BodyType
		[50]	BLANK (0 '\0')	Tetris::BodyType
		[51]	BLANK (0 '\0')	Tetris::BodyType
		[52]	BLANK (0 '\0')	Tetris::BodyType
		[53]	BLANK (0 '\0')	Tetris::BodyType
		[54]	BLANK (0 '\0')	Tetris::BodyType
		[55]	BLANK (0 '\0')	Tetris::BodyType
		[56]	BLANK (0 '\0')	Tetris::BodyType
		[57]	BLANK (0 '\0')	Tetris::BodyType
		[58]	BLANK (0 '\0')	Tetris::BodyType
"""
WIDTH = 12
INFO_INDEX = 1
def visualize_file(file_name):
    fobj = open(file_name)
    vis_str = ""
    cnt = 0
    for line in fobj:
        if (cnt % WIDTH) == 0:
            vis_str += '\n'
        cnt += 1
        arr = line.split()
        info = arr[INFO_INDEX]
        if (len(info) == 1):
            # The alphabet
            vis_str += info
        elif (info == "BLANK"):
            vis_str += ' '
        elif (info == "BORDER"):
            vis_str += '#'
        else:
            vis_str += '?'
    print(vis_str)

visualize_file(input("Enter file name: "))
