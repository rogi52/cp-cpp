// int B = sqrt(N); // bucket size
// int NB = (N + B - 1) / B; // num of bucket

// // [L, R) query
// int i = L;
// for(; i < R && i % B != 0; i++) { /* data[i] */ }
// for(; i + B <= R; i += B) { /* bucket[i / B] */ }
// for(; i < R; i++) { /* data[i] */ }
