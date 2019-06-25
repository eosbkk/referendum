void auditorbos::stake( name from,
                        name to,
                        asset quantity,
                        const std::string& memo ) {

    if (to == _self) {
        const auto & reg_candidate = registered_candidates.find(from.value);
        const time_point_sec unstaking_period = current_time_point() + time_point_sec(configs().lockup_release_time_delay);

        // Modify existing registered candidate with additional locked_tokens
        if (reg_candidate != registered_candidates.end()) {
            registered_candidates.modify(reg_candidate, eosio::same_payer, [&](candidate & row) {
                row.locked_tokens += quantity;
                row.unstaking_end_time_stamp = unstaking_period;
            });
        // New candidate, register them and add locked_tokens
        } else {
            registered_candidates.emplace(_self, [&](candidate & row) {
                row.candidate_name = from;
                row.locked_tokens = quantity;
                row.total_votes = 0;
                row.is_active = 0;
                row.unstaking_end_time_stamp = unstaking_period;
            });
        }
    }
}