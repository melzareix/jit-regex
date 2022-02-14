import random
import string


def get_random_unicode(length):

    try:
        get_char = unichr
    except NameError:
        get_char = chr

    # Update this to include code point ranges to be sampled
    include_ranges = [
        (0x0021, 0x0021),
        (0x0023, 0x0026),
        (0x0028, 0x007E),
        (0x00A1, 0x00AC),
        (0x00AE, 0x00FF),
        (0x0100, 0x017F),
        (0x0180, 0x024F),
        (0x2C60, 0x2C7F),
        (0x16A0, 0x16F0),
        (0x0370, 0x0377),
        (0x037A, 0x037E),
        (0x0384, 0x038A),
        (0x038C, 0x038C),
    ]

    alphabet = [
        get_char(code_point)
        for current_range in include_ranges
        for code_point in range(current_range[0], current_range[1] + 1)
    ]
    s = ""
    for i in range(length):
        x = random.choice(alphabet)
        s += x
        if (i + 1) % 1000 == 0:
            s += '\n'
            f.write(s)
            s = ""

    if len(s) > 0:
        f.write(s + '\n')

    return s


def get_random_ascii(sz):
    s = ""
    for i in range(sz):
        x = random.choice(string.ascii_letters)
        s += x
        if (i + 1) % 1000 == 0:
            s += '\n'
            f.write(s)
            s = ""

    if len(s) > 0:
        f.write(s + '\n')
    return s


if __name__ == "__main__":
    sz = 1024 * 1024 * 50
    # with open(f"unicode_{(sz*2)//(1024*1024)}mb.txt", "w") as f:
        # f.write(get_random_unicode(sz))

    # print("unicode written")
    with open(f"ascii_{(sz*2)//(1024*1024)}mb.txt", "w") as f:
        get_random_ascii(sz * 2)
