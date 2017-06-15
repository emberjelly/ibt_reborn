

directory = '../beats_on_the_barbie/beat_database/open/'
files = dir('../beats_on_the_barbie/beat_database/open/*.csv')

file_names = {files.name}
open_training_set = {};

for i = 1:length(file_names)
    current_data = csvread(strcat(directory, file_names{i}));
    open_training_set{i} = current_data;
end


for i = 1:length(file_names)
    current_data = open_training_set{i};
    beats_times = current_data(:, 1);
    beat_data = current_data(:, 2:end);
    
    energies = [];
    figure
    for j = 1:length(beats_times)
        energies = [energies sum(beat_data(j, :)).^2];
    end
    
    energies = energies(1:length(energies) - mod(length(energies), 4));
    energies = reshape(energies, 4, []);
    stem(sum(energies, 2));
    
end


