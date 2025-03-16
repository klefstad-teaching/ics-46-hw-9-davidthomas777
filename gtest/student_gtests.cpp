#include <gtest/gtest.h>
#include "dijkstras.h"
#include "ladder.h"
#include <sstream>

class DijkstraTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
    
    Graph createSimpleGraph() {
        std::stringstream ss;
        ss << "4\n";
        ss << "0 1 5\n";
        ss << "0 2 3\n";
        ss << "1 3 2\n";
        ss << "2 1 1\n";
        ss << "2 3 6\n";
        
        Graph G;
        ss >> G;
        return G;
    }
    
    Graph createDisconnectedGraph() {
        std::stringstream ss;
        ss << "4\n";
        ss << "0 1 2\n";
        ss << "1 0 2\n";
        
        Graph G;
        ss >> G;
        return G;
    }
};

TEST_F(DijkstraTest, SimpleGraphTest) {
    Graph G = createSimpleGraph();
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);
    
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 4);
    EXPECT_EQ(distances[2], 3);
    EXPECT_EQ(distances[3], 6);
    
    vector<int> path_to_3 = extract_shortest_path(distances, previous, 3);
    vector<int> expected_path = {0, 2, 1, 3};
    EXPECT_EQ(path_to_3, expected_path);
}

TEST_F(DijkstraTest, DisconnectedGraphTest) {
    Graph G = createDisconnectedGraph();
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);
    
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 2);
    EXPECT_EQ(distances[2], INF);
    EXPECT_EQ(distances[3], INF);
    
    vector<int> path_to_2 = extract_shortest_path(distances, previous, 2);
    EXPECT_TRUE(path_to_2.empty());
}

TEST_F(DijkstraTest, SingleNodePathTest) {
    Graph G = createSimpleGraph();
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);
    
    vector<int> path_to_0 = extract_shortest_path(distances, previous, 0);
    vector<int> expected_path = {0};
    EXPECT_EQ(path_to_0, expected_path);
}

TEST_F(DijkstraTest, LoadFromFileTest) {
    try {
        Graph G;
        file_to_graph("small.txt", G);
        
        EXPECT_GT(G.numVertices, 0);
        
        vector<int> previous;
        vector<int> distances = dijkstra_shortest_path(G, 0, previous);
        
        EXPECT_EQ(distances.size(), G.numVertices);
    } catch (const exception& e) {
        FAIL() << "Exception thrown during file loading: " << e.what();
    }
}

TEST_F(DijkstraTest, NegativeEdgeWeightsTest) {
    std::stringstream ss;
    ss << "3\n";
    ss << "0 1 2\n";
    ss << "1 2 -3\n";
    
    Graph G;
    ss >> G;
    
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);
    
    EXPECT_EQ(distances[2], -1);
}

class LadderTest : public ::testing::Test {
protected:
    void SetUp() override {
        load_words(word_list, "words.txt");
    }

    set<string> word_list;
};

TEST_F(LadderTest, SameWordTest) {
    vector<string> ladder = generate_word_ladder("test", "test", word_list);
    EXPECT_TRUE(ladder.empty());
}

TEST_F(LadderTest, BasicLadderTest) {
    vector<string> ladder = generate_word_ladder("cat", "dog", word_list);
    EXPECT_EQ(ladder.size(), 4);
    
    if (!ladder.empty()) {
        EXPECT_EQ(ladder.front(), "cat");
        EXPECT_EQ(ladder.back(), "dog");
    }
    
    for (size_t i = 1; i < ladder.size(); i++) {
        EXPECT_TRUE(is_adjacent(ladder[i-1], ladder[i]));
    }
}

TEST_F(LadderTest, DifferentLengthWordsTest) {
    vector<string> ladder = generate_word_ladder("car", "cheat", word_list);
    EXPECT_EQ(ladder.size(), 4);
    
    if (!ladder.empty()) {
        EXPECT_EQ(ladder.front(), "car");
        EXPECT_EQ(ladder.back(), "cheat");
    }
    
    for (size_t i = 1; i < ladder.size(); i++) {
        EXPECT_TRUE(is_adjacent(ladder[i-1], ladder[i]));
    }
}

TEST_F(LadderTest, NonDictionaryStartWordTest) {
    vector<string> ladder = generate_word_ladder("marty", "curls", word_list);
    EXPECT_EQ(ladder.size(), 6);
    
    if (!ladder.empty()) {
        EXPECT_EQ(ladder.front(), "marty");
        EXPECT_EQ(ladder.back(), "curls");
    }
}

TEST_F(LadderTest, EditDistanceTest) {
    EXPECT_TRUE(edit_distance_within("cat", "cot", 1));
    EXPECT_TRUE(edit_distance_within("cat", "cats", 1));
    EXPECT_FALSE(edit_distance_within("cat", "dog", 1));
    EXPECT_FALSE(edit_distance_within("cat", "carts", 1));
}

TEST_F(LadderTest, IsAdjacentTest) {
    EXPECT_TRUE(is_adjacent("cat", "cot"));
    EXPECT_TRUE(is_adjacent("chat", "cheat"));
    EXPECT_TRUE(is_adjacent("cat", "cats"));
    EXPECT_FALSE(is_adjacent("cat", "dog"));
}

TEST_F(LadderTest, VerificationExamplesTest) {
    EXPECT_EQ(generate_word_ladder("code", "data", word_list).size(), 6);
    EXPECT_EQ(generate_word_ladder("work", "play", word_list).size(), 6);
    EXPECT_EQ(generate_word_ladder("sleep", "awake", word_list).size(), 8);
}

TEST_F(LadderTest, NoLadderTest) {
    vector<string> ladder = generate_word_ladder("xyz", "aardvark", word_list);
    EXPECT_TRUE(ladder.empty());
}

TEST_F(LadderTest, CaseInsensitiveTest) {
    vector<string> ladder1 = generate_word_ladder("cat", "dog", word_list);
    vector<string> ladder2 = generate_word_ladder("CAT", "DOG", word_list);
    
    EXPECT_EQ(ladder1.size(), ladder2.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}