c = 'ä'
alt_char = 'ß'
with open('/home/elzarei/bench_data/bad_case.txt', 'w') as f:
    for i in range(500):
        line = ''
        per_line = 600 * 1024 * 10
        correct_seq_len = 50
        range_ = (per_line // correct_seq_len)
        for j in range(range_ - 1):
            line += ''.join([c for _ in range(correct_seq_len - 1)])
            line += alt_char
        line += ''.join([c for _ in range(correct_seq_len)])
        f.write(line + '\n')
        # print(len(line), len(line.encode('utf-8')))