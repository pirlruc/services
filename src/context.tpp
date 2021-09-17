/**
 * @brief Construct a new improc::Context<key type> object
 * 
 * @tparam key_type 
 */
template <typename key_type>
improc::Context<key_type>::Context() : improc::Container<key_type,std::any>() {}
