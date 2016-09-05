def skin_complexion(genome):
    """Attempts to deduce skin complexion from rs1426654."""
    snp = genome.rs1426654

    if snp == "AA":
        return "Likely light-skinned of European descent"

    if snp == "GA" or snp == "AG":
        return "Possibly mixed African-European ancestry"

    if snp == "GG":
        return "Likely dark-skinned of Asian or African ancestry"

    return "Unknown"
