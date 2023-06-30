The Core::TextReader and Core::TextFragment APIs can be used for parsing lines of text and sorting through delimiters.

Source/core/TextFragment.h, Source/core/TextFragment.cpp, Source/core/TextReader.h and Source/core/TextReader.cpp 

Below an example of using the API, from Communicator.cpp.

                // Parse it line, by line...
                Core::DataElementFile bufferFile(filename, Core::File::USER_READ);
                Core::TextReader reader(bufferFile);

                while (reader.EndOfText() == false) {
                    Core::TextFragment line(reader.ReadLine());

                    // Drop the spaces in the begining...
                    line.TrimBegin(" \t");

                    if ((line.IsEmpty() == false) && (line[0] != '#')) {
                        Core::TextSegmentIterator segments(line, true, " \t");

                        if (segments.Next() == true) {

                            // Looks like we have a word, see what the word is...
                            if ((segments.Current() == _T("include")) && (segments.Next() == true)) {
                                // Oke, dive into this entry...
                                ReadList(segments.Remainder().Text(), entries);
                            }
                            else {
                                entries.emplace_back(line.Text());
                            }
                        }
                        else {
                            entries.emplace_back(line.Text());
                        }
                    }
                }
