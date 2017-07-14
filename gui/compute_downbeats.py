

def load_beat_times(fname):
	pass

def load_down_beat_times(fname):

	f = open(fname, 'r')
	data = f.read()

	pairs = data.split('\n')

	pairs.pop(-1)
	beat_nums = []
	beat_times = []

	for pair in pairs:
		p = pair.split('\t')
		beat_nums.append(p[1])
		beat_times.append(p[0])


	return (beat_times, beat_nums)


def compute_downbeats(true_beats, estimates, beat_nums):

	true_beat_nums = []
	seq_len = 0
	cur_seq_index = 0
	best_seq_index = 0
	best_seq_len = 0

	next_beat = 1
	for i, b in enumerate(estimates):
		print b
		if b == next_beat:

			next_beat = (next_beat % 4) + 1
			seq_len += 1
			if seq_len > best_seq_len:
				best_seq_len = seq_len

		else:
			next_beat = 1 #(b % 4) + 1
			seq_len = 0
			
			if b == 1:
				cur_seq_index = i
			
			# Start counting bars

	print best_seq_len
	print best_seq_index


	best_seq_beat = best_seq_index%4
	print best_seq_beat





	arr1 = true_beats
	arr2 = estimates

	sums = [0, 0, 0, 0]
	for i in range(min(len(arr1), len(arr2)) - 4):
		sums[0] += abs(arr1[i] - arr2[i])
		sums[1] += abs(arr1[i] - arr2[i + 1])
		sums[2] += abs(arr1[i] - arr2[i + 2])
		sums[3] += abs(arr1[i] - arr2[i + 3])


	min_index = 0
	for i in range(len(sums)):
		if sums[i] < sums[min_index]:
			min_index = i

	print "Beat index", min_index

	result = [1, 2, 3, 4]*(len(true_beats)/4 + 1)

	for i in range(min_index):
		result.pop(0)
	return result

	'''





	for i, b in enumerate(true_beats):
		for j, b2 in enumerate(estimates):
			if estimates[j] > true_beats[i]:
				# Selected the closes from this beat and the previous

				if j != 0:
					if abs(estimates[j] - true_beats[i]) < abs(estimates[j - 1] - true_beats[i]):
						index = j
					else:
						index = j - 1
				else:
					index = j
				break

		print true_beats[i], estimates[index], beat_nums[index]
		true_beat_nums.append(beat_nums[index])

	## This code assumes the time signature is constant throughout the piece
	seq_len = 0
	cur_seq_index = 0
	best_seq_index = 0
	best_seq_len = 0

	i = 0
	next_beat = 1
	# First do 4/4


	for i, b in enumerate(true_beat_nums):
		print b
		if b == next_beat:

			next_beat = (next_beat % 4) + 1
			seq_len += 1
			print next_beat
			if seq_len > best_seq_len:
				best_seq_len = seq_len

		else:
			next_beat = 1 #(b % 4) + 1
			seq_len = 0
			
			if b == 1:
				cur_seq_index = i
			
			# Start counting bars

	print best_seq_len
	print best_seq_index


	best_seq_beat = best_seq_index%4
	print best_seq_beat


	result = [1, 2, 3, 4]*(len(true_beats)/4 + 1)

	for i in range(best_seq_beat):
		result.pop(0)


	return result
	return true_beat_nums

	'''



