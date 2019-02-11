import os, sys
import json
import numpy as np

'''glabal variables'''
raw_path = 'raw'
offset = [0, 15000, 30000, 45000, 60000, 64767, 79767, 94767, 109767, 124767]
total_length = 129154
team_dict = {'阿根廷': 0, '巴西': 1,}
team_list = ['Argentina', 'Brazil']
output_filename = ['position.txt', 'alphabet.txt', 'label.txt']
position_filename = 'position.json'

'''functions'''
def doFormats():
	# 初始化，使用的是共享字典，注释掉了分离的字典
	# format_sets = [set(), set()]
	format_set = set()
	format_dict = {'None': 0}
	formats = [[],[]]

	filepaths = [os.path.join(raw_path, d) for d in os.listdir(raw_path)]
	# 首先遍历文件 读入并进行处理(处理offset0, 将所有文件的数据连起来
	for file_no, filepath in enumerate(filepaths):
		with open(filepath, 'r', encoding='UTF-8') as file:
			data = json.load(file)
		for team_no, team in enumerate(data['team']):
			# 获取数据对应的球队编号
			team_id = team_dict[data['team'][team_no]]
			# 按照 ( 阵型, 开始时间, 结束时间 ) 的格式截取数据
			format_list = [(f['format'], f['startTime'] + offset[file_no], f['endTime'] + offset[file_no]) for f in data["formats"][team_no]]
			# 用列表生成器生成纯阵型列表 ( 用于建立alphabet )
			pure_formats = [f['format'] for f in data["formats"][team_no]]
			# 更新无序集 set
			format_set.update(pure_formats)
			# format_sets[team_id].update(pure_formats)
			# 按照 team_id 连接数据
			formats[team_id] += format_list
	# 遍历完成, 用合并后的 set 建立 alphabet
	for index, format in enumerate(format_set):
		format_dict[format] = index + 1
	# 从字典生成列表便于输出
	format_dict_list =  list(format_dict)
	print(format_dict_list)
	# 分队标注并输出
	for team_id, team_data in enumerate(formats):
		# 根据队伍构造输出文件路径
		output_path = [os.path.join(team_list[team_id], d) for d in output_filename]
		# 输出alphabet
		writeList(output_path[1], format_dict_list)
		# 创建全零数组(默认无阵型), 
		label = np.zeros((total_length,), dtype = 'int')
		# 遍历所有阵型, 通过下标分片赋值
		for fmt in formats[team_id]:
			start = fmt[1]
			end = fmt[2]
			label[start: end] = format_dict[fmt[0]]
		# 保存label文件
		np.savetxt(output_path[2], label, fmt = '%d')
	pass

def doPosition():
	# 读位置文件
	with open(position_filename, 'r') as file:
		data = json.load(file)
	# 创建一支队伍的空数组
	empty_arr = np.empty((total_length, 10, 2), dtype = 'int')
	# 两支队两个数组
	position = [empty_arr, empty_arr]
	# 遍历读入的数据, 提取有效信息
	for frame, obj in enumerate(data):
		# 先按时间遍历, 再按队伍遍历
		for team_id, team in enumerate(team_list):
			# 每一帧取前10个位置数据构成列表
			team_data = [(d['y'], d['x']) for d in obj[team]][0: 10]
			# 写入列表
			position[team_id][frame] = team_data
			pass
	# 数据遍历完成, 按照队伍输出
	for team_id, team in enumerate(team_list):
		output_path = [os.path.join(team, d) for d in output_filename]
		write(output_path[0], position[team_id])
	pass

def writeList(filepath, data):
	with open(filepath, 'w+') as file:
		for item in data:
			file.write(item)
			file.write('\n')
	pass


def write(file, data):
	with open(file,'w+') as outfile:
		# I'm writing a header here just for the sake of readability
		# Any line starting with "#" will be ignored by numpy.loadtxt
		outfile.write('# Array shape: {0}\n'.format(data.shape))

		# Iterating through a ndimensional array produces slices along
		# the last axis. This is equivalent to data[i,:,:] in this case
		for data_slice in data:
			# for data_slice in data_slice0:
			# The formatting string indicates that I'm writing out
			# the values in left-justified columns 7 characters in width
			# with 2 decimal places.  
			np.savetxt(outfile, data_slice, fmt='%-7d')

			# Writing out a break to indicate different slices...
			outfile.write('# New slice\n')


if __name__ == '__main__':
	doFormats()
	doPosition()


